//
// Created by physi on 18/12/2024.
//

#include "World.h"

#include <algorithm>
#include <format>

#include "WorldGenerator.h"
#include "../Player/Player.h"
#include "../Utils/Logger.h"
#include "../Application/AppEvents.h"
#include "../Application/Application.h"
#include "../Rendering/TextureManager.h"

namespace BlockGame {

    int renderDistance = 6;

    constexpr float maxSun = 1500.0f;
    constexpr float minSun = 10.0f;
    constexpr uint8_t LAMP_LIGHT_LEVEL = 16;

    // Tests the collision between a block and a position.
    bool TestAABB3DCollision(const glm::vec3& pos_1, const glm::vec3& dim_1, const glm::vec3& pos_2, const glm::vec3& dim_2)
    {
        if (pos_1.x < pos_2.x + dim_2.x &&
            pos_1.x + dim_1.x > pos_2.x &&
            pos_1.y < pos_2.y + dim_2.y &&
            pos_1.y + dim_1.y > pos_2.y &&
            pos_1.z < pos_2.z + dim_2.z &&
            pos_1.z + dim_1.z > pos_2.z)
        {
            return true;
        }

        return false;
    }

    // Converts world position to chunk block position
    static glm::ivec3 WorldBlockToLocalBlockCoordinates(const glm::vec3& pos)
    {
        int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
        int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));

        int bx = static_cast<int>(floor(pos.x)) - (block_chunk_x * CHUNK_SIZE_X);
        int by = static_cast<int>(floor(pos.y)); // Y-coordinate should already be valid
        int bz = static_cast<int>(floor(pos.z)) - (block_chunk_z * CHUNK_SIZE_Z);

        return glm::ivec3(bx, by, bz);
    }

    World::World(int seed, const glm::vec2 &windowSize, const std::string &worldName)
        : m_WorldSeed(seed), m_WorldName(worldName), m_ViewFrustum(), m_GUICamera(0.0f, windowSize.x, 0.0f, windowSize.y)
    {
        BlockDatabase::Initialize();
        TextureManager::Initialize();

        // Create player
        p_Player = new Player(windowSize);

        // Set player position
        p_Player->SetPosition(glm::vec3(0, 10, 0));

        // Crosshair
        m_CrosshairTexture.CreateTexture(std::string(RESOURCE_PATH) + "/Textures/GUI/crosshair.png", false);
        m_CrosshairPosition = glm::vec2(windowSize.x / 2.0f, windowSize.y / 2.0f);

        m_GUICamera.SetProjection(0.0f, windowSize.x, 0.0f, windowSize.y);

        Logger::Debug("World is constructing");

        // crosshair
        // other ui stuff etc.

        Chunk* chunk1 = EmplaceChunk(0, 0);
        Chunk* chunk2 = EmplaceChunk(1, 0);
        GenerateChunk(chunk1, 0);
        GenerateChunk(chunk2, 0);
    }

    World::~World() = default;

    void World::Update(GLFWwindow *window, float deltaTime)
    {
        // Get player's current chunk
        glm::vec3 playerPosition = p_Player->GetPosition();
        int playerChunkX = (int)floor(playerPosition.x / CHUNK_SIZE_X);
        int playerChunkZ = (int)floor(playerPosition.z / CHUNK_SIZE_Z);

        // Set the distance for building chunks (further than render distance)
        int buildDistance = renderDistance + 4;

        // for (int i = playerChunkX - buildDistance; i < playerChunkX + buildDistance; i++)
        // {
        //     for (int j = playerChunkZ - buildDistance; j < playerChunkZ + buildDistance; j++)
        //     {
        //         if (IsChunkValid(i, j) == false)
        //         {
        //             Chunk* chunk = EmplaceChunk(i, j);
        //             GenerateChunk(chunk, m_WorldSeed);
        //         }
        //     }
        // }

        // Update player
        p_Player->Update(window, deltaTime);

        // Update view frustum
        m_ViewFrustum.Update(p_Player->GetCamera().GetProjectionMatrix());

        // Increase frame count
        m_CurrentFrame++;

        // TODO: Perform clean up every x frames....
        // TODO: more...


    }

    void World::RenderWorld()
    {
        static float ambientLight = 0.4f;

        // TODO: needed for skybox: glDisable(GL_CULL_FACE);
        // TODO: perform skybox here

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        // Determine player's current chunk
        int playerChunkX = (int) floor(p_Player->GetPosition().x / CHUNK_SIZE_X);
        int playerChunkZ = (int) floor(p_Player->GetPosition().z / CHUNK_SIZE_Z);
        uint32_t renderedChunks = 0;

        for (int i = playerChunkX - renderDistance; i < playerChunkX + renderDistance; i++)
        {
            for (int j = playerChunkZ - renderDistance; j < playerChunkZ + renderDistance; j++)
            {
                // Check if chunk exists or create it if needed
                if (!IsChunkValid(i, j))
                {
                    continue;
                    //EmplaceChunk(i, j);
                }

                // Get the chunk
                Chunk* chunk = GetChunkAt(i, j);
                if (!chunk)
                {
                    Logger::Error(std::format("Chunk at {}, {} is missing!", i, j));
                    continue;
                }

                m_Renderer.RenderChunk(chunk,
                                           p_Player->GetCamera().GetViewMatrix(),
                                           p_Player->GetCamera().GetProjectionMatrix());
                renderedChunks++;

                // Frustum culling check
                if (m_ViewFrustum.IsBoxIntersectingFrustum(chunk->ChunkFrustumAABB))
                {

                }
            }
        }

        p_ChunksRendered = renderedChunks;

        if (p_Player->p_bIsHoveringBlock)
        {
            m_Renderer.GetCubeRenderer()->RenderOutline(
                p_Player->p_HoverBlockPosition,
                p_Player->GetCamera().GetViewMatrix(),
                p_Player->GetCamera().GetProjectionMatrix()
            );
        }


        // TODO: Now render all the transparency
        // 1. glDisable(GL_CULL_FACE);
        // same loop but do a transparent pass

        //m_Renderer.EndChunkRendering();

        // then chunk models (grass etc.)

        // Then finally render 2d elements like the crosshair

        m_GUIRenderer.RenderQuad(glm::vec2(m_CrosshairPosition.x - (17.5f),
                                            m_CrosshairPosition.y - (17.5f)),
                                            &m_CrosshairTexture,
                                            &m_GUICamera, 35, 35, 1);
    }

    void World::OnEvent(AppEvents::AppEvent e)
    {
        //p_Player->OnEvent(e);

        if (e.type == AppEvents::WindowResize)
        {
            if (e.windowWidth > 0 && e.windowHeight > 0)
            {
                p_Player->GetCamera().UpdateAspectRatio(e.windowWidth, e.windowHeight);
                m_GUICamera.SetProjection(static_cast<float>(0), static_cast<float>(e.windowWidth), static_cast<float>(0), static_cast<float>(e.windowHeight));

                m_CrosshairPosition = glm::vec2(e.windowWidth / 2.0, e.windowHeight / 2.0);

                // update 2d camera
                // update UI
            }
        }
        else if (e.type == AppEvents::KeyDown)
        {
            if (e.key == GLFW_KEY_ESCAPE && GameApplication->GetState() == GameState::GS_PAUSED_GAME)
            {
                if (p_Player)
                {
                    p_Player->GetCamera().SetInputPause(true);
                }
            }
            else if (e.key == GLFW_KEY_ESCAPE && GameApplication->GetState() == GameState::GS_PLAYING_GAME)
            {
                if (p_Player)
                {
                    p_Player->GetCamera().SetInputPause(false);
                }
            }
        }
    }

    void World::SetRenderDistance(int val)
    {
        renderDistance = val;
    }

    ChunkBlock World::GetBlockAt(const glm::vec3 &position) noexcept
    {
        // Offset by -0.5 to handle centered blocks
        glm::vec3 adjustedPosition = position + glm::vec3(0.5f);

        // Compute chunk coordinates
        int blockChunkX = static_cast<int>(std::floor(adjustedPosition.x / CHUNK_SIZE_X));
        int blockChunkZ = static_cast<int>(std::floor(adjustedPosition.z / CHUNK_SIZE_Z));

        // Compute local block coordinates within the chunk
        int bx = static_cast<int>(std::floor(adjustedPosition.x)) % CHUNK_SIZE_X;
        int by = static_cast<int>(std::floor(adjustedPosition.y));
        int bz = static_cast<int>(std::floor(adjustedPosition.z)) % CHUNK_SIZE_Z;

        // Ensure local coordinates are non-negative
        if (bx < 0) bx += CHUNK_SIZE_X;
        if (bz < 0) bz += CHUNK_SIZE_Z;

        // Fetch the chunk
        Chunk* chunk = GetChunkAt(blockChunkX, blockChunkZ);

        // If chunk is missing or y is out of range, return null
        if (!chunk || by < 0 || by >= CHUNK_SIZE_Y)
        {
            return ChunkBlock();
        }

        // Return the block and the chunk
        return ChunkBlock(chunk, &chunk->ChunkContents[bx][by][bz], glm::vec3(bx, by, bz));
    }

    void World::SetBlockAt(BlockType type, const glm::vec3 &position) noexcept
    {
        // Offset by +0.5 to handle centered blocks
        glm::vec3 adjustedPosition = position + glm::vec3(0.5f);

        // Compute chunk coordinates
        int blockChunkX = static_cast<int>(std::floor(adjustedPosition.x / CHUNK_SIZE_X));
        int blockChunkZ = static_cast<int>(std::floor(adjustedPosition.z / CHUNK_SIZE_Z));

        // Compute local block coordinates within the chunk
        int bx = static_cast<int>(std::floor(adjustedPosition.x)) % CHUNK_SIZE_X;
        int by = static_cast<int>(std::floor(adjustedPosition.y));
        int bz = static_cast<int>(std::floor(adjustedPosition.z)) % CHUNK_SIZE_Z;

        // Ensure local coordinates are non-negative
        if (bx < 0) bx += CHUNK_SIZE_X;
        if (bz < 0) bz += CHUNK_SIZE_Z;

        // Fetch the chunk
        Chunk* chunk = GetChunkAt(blockChunkX, blockChunkZ);

        // If chunk is missing or y is out of range, return null
        if (!chunk || by < 0 || by >= CHUNK_SIZE_Y)
        {
            return;
        }

        chunk->SetBlock(type, glm::vec3(bx, by, bz));
        Logger::Debug(std::format("Set block at ({}, {}, {}) to {}", position.x, position.y, position.z, BlockTypeToString(type)));
    }

    bool World::BreakBlockAt(const glm::vec3 &position) noexcept
    {
        ChunkBlock block = GetBlockAt(position);
        if (block.OwnerChunk && block.Block->type != BlockType::Air)
        {
            SetBlockAt(BlockType::Air, position);
            return true;
        }

        return false;
    }

    bool World::PlaceBlockAt(const glm::vec3 &position, BlockType type) noexcept
    {
        ChunkBlock block = GetBlockAt(position);
        if (block.OwnerChunk && block.Block->type == BlockType::Air)
        {
            SetBlockAt(type, position);
            return true;
        }

        return false;
    }

    BlockType World::GetBlockTypeAt(const glm::vec3 &position) noexcept
    {
        int blockChunkX = static_cast<int>(floor(position.x / CHUNK_SIZE_X));
        int blockChunkZ = static_cast<int>(floor(position.z / CHUNK_SIZE_Z));
        int bx = position.x - (blockChunkX * CHUNK_SIZE_X);
        int by = static_cast<int>(floor(position.y));
        int bz = position.z - (blockChunkZ * CHUNK_SIZE_Z);

        return static_cast<BlockType>(GetChunkAt(blockChunkX, blockChunkZ)->ChunkContents.at(bx).at(by).at(bz).type);
    }

    Chunk* World::GetChunkAt(int cx, int cz) noexcept
    {
        auto it = m_WorldChunks.find(std::make_pair(cx, cz));

        if (it == m_WorldChunks.end())
        {
            // Safely return nullptr if the chunk is missing
            return nullptr;
        }

        // Safe access to the chunk
        return &it->second;
    }

    bool World::IsChunkValid(int cx, int cz)
    {
        std::map<std::pair<int, int>, Chunk>::iterator chunkIsValid = m_WorldChunks.find(std::pair<int,int>(cx, cz));

        if (chunkIsValid == m_WorldChunks.end())
        {
            return false;
        }

        return true;
    }

    Chunk* World::EmplaceChunk(int cx, int cz)
    {
        Logger::Debug(std::format("Building new chunk at X: {} Z: {}", cx, cz));

        // Insert if missing, or access the existing chunk
        auto [it, inserted] = m_WorldChunks.try_emplace(std::make_pair(cx, cz), glm::vec3(cx, 0, cz));

        if (inserted) {
            p_ChunkCount++;
        }

        return &it->second;
    }

    int World::GetSeed()
    {
        return m_WorldSeed;
    }

    LineTraceResult World::LineTrace(const glm::vec3& start, const glm::vec3& direction,
        float maxDistance, float stepSize)
    {
        glm::vec3 currentPos = start;
        glm::vec3 faceNormal = glm::vec3(0);

        for (float distance = 0.0f; distance <= maxDistance; distance += stepSize)
        {
            auto chunkBlock = GetBlockAt(currentPos);

            if (chunkBlock.OwnerChunk && chunkBlock.Block && chunkBlock.Block->type != BlockType::Air)
            {
                // Block bounds (centered on the coordinate with bounds of 0.5)
                glm::vec3 blockMin = chunkBlock.BlockPosition - glm::vec3(0.5f);
                glm::vec3 blockMax = chunkBlock.BlockPosition + glm::vec3(0.5f);

                // Calculate the intersection point
                glm::vec3 hitPoint = currentPos;

                // Determine which face was hit by comparing distances
                float dxMin = std::abs(hitPoint.x - blockMin.x);
                float dxMax = std::abs(hitPoint.x - blockMax.x);
                float dyMin = std::abs(hitPoint.y - blockMin.y);
                float dyMax = std::abs(hitPoint.y - blockMax.y);
                float dzMin = std::abs(hitPoint.z - blockMin.z);
                float dzMax = std::abs(hitPoint.z - blockMax.z);

                // Determine the closest face
                float minDistance = dxMin;
                faceNormal = glm::vec3(-1.0f, 0.0f, 0.0f); // Left face

                if (dxMax < minDistance)
                {
                    minDistance = dxMax;
                    faceNormal = glm::vec3(1.0f, 0.0f, 0.0f); // Right face
                }
                if (dyMin < minDistance)
                {
                    minDistance = dyMin;
                    faceNormal = glm::vec3(0.0f, -1.0f, 0.0f); // Bottom face
                }
                if (dyMax < minDistance)
                {
                    minDistance = dyMax;
                    faceNormal = glm::vec3(0.0f, 1.0f, 0.0f); // Top face
                }
                if (dzMin < minDistance)
                {
                    minDistance = dzMin;
                    faceNormal = glm::vec3(0.0f, 0.0f, -1.0f); // Back face
                }
                if (dzMax < minDistance)
                {
                    minDistance = dzMax;
                    faceNormal = glm::vec3(0.0f, 0.0f, 1.0f); // Front face
                }

                return { true, chunkBlock, currentPos, faceNormal };
            }

            currentPos += direction * stepSize;
        }

        return { false, ChunkBlock(), currentPos, faceNormal };
    }

    void World::UnloadFarChunks()
    {

    }
} // BlockGame