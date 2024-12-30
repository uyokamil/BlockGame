//
// Created by physi on 18/12/2024.
//

#include "WorldGenerator.h"

#include <format>

#include "../Utils/Logger.h"
#include "../Utils/Timer.h"

namespace BlockGame {
    void GenerateChunk(Chunk *chunk, const int WorldSeed)
    {
        Logger::Debug(std::format("WorldGenerator::GenerateChunk(): Generating Chunk at x{} y{} z{}...", chunk->Position.x, chunk->Position.y, chunk->Position.z));
        Timer timer;

        for (int x = 1; x < CHUNK_SIZE_X-1; ++x) {
            for (int z = 1; z < CHUNK_SIZE_Z-1; ++z) {
                for (int y = 0; y <= 10; ++y) {
                    if (y < 8)
                    {
                        chunk->SetBlock(BlockType::Stone, glm::vec3(x, y, z));
                    }
                    else if (y < 10)
                    {
                        chunk->SetBlock(BlockType::Dirt, glm::vec3(x, y, z));
                    }
                    else
                    {
                        chunk->SetBlock(BlockType::Grass, glm::vec3(x, y, z));
                    }
                }
            }
        }
    }
} // BlockGame