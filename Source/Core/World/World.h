//
// Created by physi on 18/12/2024.
//

#ifndef WORLD_H
#define WORLD_H
#include <map>
#include <string>
#include <glm/vec2.hpp>

#include "../Application/AppEvents.h"
#include "../Blocks/Block.h"
#include "../Chunks/Chunk.h"
#include "../Rendering/GUIRenderer.h"
#include "../Rendering/Renderer.h"

namespace BlockGame {
    class Player;

    struct ChunkBlock
    {
        ChunkBlock(Chunk* c, Block* b, glm::vec3 pos)
            : OwnerChunk(c), Block(b), BlockPosition(pos) {}

        ChunkBlock()
        {
            OwnerChunk = nullptr;
            Block = nullptr;
            BlockPosition = glm::vec3(0, 0, 0);
        }

        Chunk* OwnerChunk;
        Block* Block;
        glm::vec3 BlockPosition;
    };

    struct LineTraceResult
    {
        LineTraceResult(bool success, ChunkBlock hitBlock, glm::vec3 hitPos, glm::vec3 hitNormal)
            : HitSuccess(success), HitBlock(hitBlock), AccurateHitPosition(hitPos), HitNormal(hitNormal) {}

        bool HitSuccess;
        ChunkBlock HitBlock;
        glm::vec3 AccurateHitPosition;
        glm::vec3 HitNormal;
    };

    class World {
    public:
        World(int seed, const glm::vec2& windowSize, const std::string& worldName);
        ~World();

        void Update(GLFWwindow* window, float deltaTime);
        void RenderWorld();
        void OnEvent(AppEvents::AppEvent e);
        void SetRenderDistance(int val);
        inline const std::string& GetName() noexcept { return m_WorldName; }

        BlockType GetBlockTypeAt(const glm::vec3& position) noexcept;
        Chunk* GetChunkAt(int cx, int cz) noexcept;
        // world gen?

        bool IsChunkValid(int cx, int cz);

        // Forceful Block Operations
        ChunkBlock GetBlockAt(const glm::vec3& position) noexcept;
        void SetBlockAt(BlockType type, const glm::vec3& position) noexcept;

        // Physics Block Operations
        bool BreakBlockAt(const glm::vec3& position) noexcept;
        bool PlaceBlockAt(const glm::vec3& position, BlockType type) noexcept;

        Player* p_Player;

        const std::map<std::pair<int, int>, Chunk>& GetWorldData()
        {
            return m_WorldChunks;
        }

        Chunk* EmplaceChunk(int cx, int cz);
        int GetSeed();

        // sun?

        uint32_t p_ChunkCount = 0;
        uint32_t p_ChunksRendered = 0;

        // Line trace
        LineTraceResult LineTrace(const glm::vec3& start, const glm::vec3& direction, float maxDistance, float stepSize = 0.1f);

        // Vectors
        static glm::vec3 WorldUp() { return glm::vec3(0.0f, 1.0f, 0.0f); }

    private:
        const std::string m_WorldName;
        const int m_WorldSeed;

        void UnloadFarChunks();

        // Rendering
        Renderer m_Renderer;
        GUIRenderer m_GUIRenderer;

        GUICamera m_GUICamera;

        std::map<std::pair<int, int>, Chunk> m_WorldChunks;

        long long m_CurrentFrame;

        // Lighting
        // TODO: lighting

        ViewFrustum m_ViewFrustum;

        // Crosshair
        Texture m_CrosshairTexture;
        glm::vec2 m_CrosshairPosition;
    };

} // BlockGame

#endif //WORLD_H
