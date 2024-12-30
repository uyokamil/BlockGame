//
// Created by physi on 16/12/2024.
//

#ifndef CHUNK_H
#define CHUNK_H
#include <array>
#include <vector>
#include <glm/vec3.hpp>

#include "../Rendering/Frustum.h"
#include "../Blocks/Block.h"

namespace OpenGLClasses
{
    class Shader;
}

namespace BlockGame
{
    using namespace OpenGLClasses;

    struct Block;
    enum class BlockType : uint8_t;

    enum class ChunkState : uint8_t
    {
        Ungenerated = 0,
        Generated   = 1,
        Changed     = 2,
        Undefined   = 3
    };

    class   Chunk {
    public:
        Chunk(glm::vec3 chunkPosition);
        ~Chunk();

        void SetBlock(BlockType type, const glm::vec3& position);
        Block* GetBlock(glm::vec3& position);

        void Construct();

        const glm::vec3 Position;
        ChunkState ChunkState = ChunkState::Ungenerated;
        std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z> ChunkContents;
        FrustumAABB ChunkFrustumAABB;

        [[nodiscard]] bool IsInBounds(const glm::ivec3& position) const;
        [[nodiscard]] bool IsBlockVisible(const glm::ivec3& pos) const;
        [[nodiscard]] vector<BlockFaceType> GetVisibleFaces(const glm::ivec3& pos) const;
        [[nodiscard]] bool IsBlockFaceVisible(const glm::ivec3& pos, BlockFaceType face) const;

    private:
        static int FloorDivision(int x, int y);
        static int Mod(int x, int y);
    };
} // BlockGame

#endif //CHUNK_H
