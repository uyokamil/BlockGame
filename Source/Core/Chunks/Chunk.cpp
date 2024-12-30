//
// Created by physi on 16/12/2024.
//

#include "Chunk.h"

#include <string.h>
#include <vector>

#include <glm/ext/matrix_transform.hpp>

#include "../Application/Application.h"
#include "../World/World.h"
#include "../Blocks/BlockDatabase.h"
#include "../OpenGL/Shader.h"
#include "../Utils/Logger.h"


namespace BlockGame
{
    Chunk::Chunk(const glm::vec3 chunkPosition)
        : Position(chunkPosition),
          ChunkContents{},
          ChunkFrustumAABB(glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z),
                 chunkPosition * glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z))
    {
        Logger::Debug(std::format("Chunk created at position x{} z{}", Position.x, Position.z));

        // Initialize all blocks to Air
        memset(&ChunkContents, 0, sizeof(ChunkContents));
    }

    Chunk::~Chunk() = default;

    void Chunk::SetBlock(BlockType type, const glm::vec3 &position)
    {
        if (IsInBounds(position))
        {
            ChunkContents[position.x][position.y][position.z] = BlockDatabase::GetBlock(type);
        }
    }

    Block* Chunk::GetBlock(glm::vec3& position)
    {
        if (IsInBounds(position))
        {
            return &ChunkContents[position.x][position.y][position.z];
        }
        return nullptr;
    }

    void Chunk::Construct()
    {
        for (int x = 0; x < CHUNK_SIZE_X; ++x)
        {
            for (int z = 0; z < CHUNK_SIZE_Z; ++z)
            {
                // Generate a layer of grass with dirt underneath
                ChunkContents[x][0][z] = BlockDatabase::GetBlock(BlockType::Cobblestone);  // Flat plane of cobblestone
            }
        }
    }

    bool Chunk::IsInBounds(const glm::ivec3 &position) const
    {
        return position.x >= 0 && position.y >= 0 && position.z >= 0 &&
               position.x < CHUNK_SIZE_X && position.y < CHUNK_SIZE_Y && position.z < CHUNK_SIZE_Z;
    }

    bool Chunk::IsBlockVisible(const glm::ivec3 &pos) const
    {
        static const vector<glm::ivec3> NeighborOffsets =
        {
            { 1,  0,  0},  // Right
            {-1,  0,  0},  // Left
            { 0,  1,  0},  // Top
            { 0, -1,  0},  // Bottom
            { 0,  0,  1},  // Front
            { 0,  0, -1}   // Back
        };

        for (const auto& offset : NeighborOffsets)
        {
            glm::ivec3 neighborPos = pos + offset;

            if (!IsInBounds(neighborPos))
            {
                // -- OUT-OF-BOUNDS CHECK
                const glm::ivec3 globalPos = glm::ivec3(Position * glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z)) + neighborPos;

                const glm::ivec2 neighborChunkPos = {
                    FloorDivision(globalPos.x, CHUNK_SIZE_X),
                    FloorDivision(globalPos.z, CHUNK_SIZE_Z)
                };

                const Chunk* neighborChunk = GameApplication->GetWorldInstance()->GetChunkAt(neighborChunkPos.x, neighborChunkPos.y);

                if (!neighborChunk)
                {
                    // If no chunk exists here yet, we'll treat this block as visible.
                    return true;
                }

                glm::ivec3 neighborChunkLocalPos =
                {
                     Mod(globalPos.x, CHUNK_SIZE_X),
                     neighborPos.y,
                     Mod(globalPos.z, CHUNK_SIZE_Z)
                };

                if (neighborChunkLocalPos.y < 0 || neighborChunkLocalPos.y >= CHUNK_SIZE_Y)
                {
                    return true; // Out of vertical bounds
                }

                const Block& neighborBlock = neighborChunk->
                    ChunkContents[neighborChunkLocalPos.x][neighborChunkLocalPos.y][neighborChunkLocalPos.z];

                if (neighborBlock.type == BlockType::Air || !neighborBlock.isOpaque)
                {
                    return true;
                }
            }
            else
            {
                // -- IN-BOUNDS CHECK

                const Block& neighborBlock = ChunkContents[neighborPos.x][neighborPos.y][neighborPos.z];
                if (neighborBlock.type == BlockType::Air || !neighborBlock.isOpaque)
                {
                    return true;
                }
            }
        }

        return false;
    }

    vector<BlockFaceType> Chunk::GetVisibleFaces(const glm::ivec3 &pos) const
    {
        std::vector<BlockFaceType> visibleFaces;

        for (BlockFaceType face : {BlockFaceType::Right, BlockFaceType::Left, BlockFaceType::Top,
                                   BlockFaceType::Bottom, BlockFaceType::Front, BlockFaceType::Back})
        {
            if (IsBlockFaceVisible(pos, face))
            {
                visibleFaces.push_back(face);
            }
        }

        return visibleFaces;
    }

    bool Chunk::IsBlockFaceVisible(const glm::ivec3 &pos, BlockFaceType face) const
    {
        static const std::unordered_map<BlockFaceType, glm::ivec3> FaceToOffset =
        {
            {BlockFaceType::Back,  {0,  0, -1}},    // NORTH
            {BlockFaceType::Front, {0,  0,  1}},    // SOUTH
            {BlockFaceType::Left,  {-1, 0,  0}},    // WEST
            {BlockFaceType::Right, {1,  0,  0}},    // EAST
            {BlockFaceType::Top,   {0,  1,  0}},    // UP
            {BlockFaceType::Bottom,{0, -1,  0}},    // DOWN
        };

        glm::ivec3 neighborPos = pos + FaceToOffset.at(face);

        if (IsInBounds(neighborPos))
        {
            // If neighbor IS within bounds, check its opacity
            const Block& neighbor = ChunkContents[neighborPos.x][neighborPos.y][neighborPos.z];
            return neighbor.type == BlockType::Air || !neighbor.isOpaque;
        }
        else
        {
            // Neighbor is out of bounds, so check the neighboring chunk
            const glm::ivec3 globalPos = glm::ivec3(Position * glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z)) + neighborPos;

            const glm::ivec2 neighborChunkPos = {
                FloorDivision(globalPos.x, CHUNK_SIZE_X),
                FloorDivision(globalPos.z, CHUNK_SIZE_Z)
            };

            const Chunk* neighborChunk = GameApplication->GetWorldInstance()->GetChunkAt(neighborChunkPos.x, neighborChunkPos.y);

            if (!neighborChunk)
            {
                // No neighboring chunk exists, so skip drawing this face.
                return false;
            }

            glm::ivec3 neighborChunkLocalPos =
            {
                Mod(globalPos.x, CHUNK_SIZE_X),
                neighborPos.y,
                Mod(globalPos.z, CHUNK_SIZE_Z)
            };

            // Validate local position bounds
            if (neighborChunkLocalPos.y < 0 || neighborChunkLocalPos.y >= CHUNK_SIZE_Y)
            {
                return true; // Out of vertical bounds
            }

            const Block& neighborBlock = neighborChunk->
                ChunkContents[neighborChunkLocalPos.x][neighborChunkLocalPos.y][neighborChunkLocalPos.z];

            return neighborBlock.type == BlockType::Air || !neighborBlock.isOpaque;
        }
    }

    int Chunk::FloorDivision(const int x, const int y)
    {
        return (x >= 0) ? (x / y) : ((x - y + 1) / y);
    }

    int Chunk::Mod(const int x, const int y)
    {
        const int result = x % y;
        return (result < 0) ? result + y : result;
    }
}
