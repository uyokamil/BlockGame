//
// Created by physi on 18/12/2024.
//

#include "Renderer.h"

#include "TextureManager.h"
#include "../Utils/Logger.h"

namespace BlockGame {
    Renderer::Renderer() = default;

    Renderer::~Renderer() = default;

    void Renderer::RenderChunk(const Chunk* chunk, const glm::mat4 &viewMatrix,
        const glm::mat4 &projectionMatrix)
    {
        for (int x = 0; x < CHUNK_SIZE_X; ++x)
        {
            for (int y = 0; y < CHUNK_SIZE_Y; ++y)
            {
                for (int z = 0; z < CHUNK_SIZE_Z; ++z)
                {
                    const Block& block = chunk->ChunkContents[x][y][z];

                    if (block.type != BlockType::Air && chunk->IsBlockVisible(glm::ivec3(x, y, z)))
                    {
                        auto visibleFaces = chunk->GetVisibleFaces({ x, y, z });
                        if (!visibleFaces.empty())
                        {
                            glm::vec3 worldPos = chunk->Position * glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z) + glm::vec3(x, y, z);
                            m_CubeRenderer.RenderCube(block, worldPos, visibleFaces, 0.0f, viewMatrix, projectionMatrix);
                        }
                    }
                }
            }
        }
    }
} // BlockGame