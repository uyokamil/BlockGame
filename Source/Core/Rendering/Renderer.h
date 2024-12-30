//
// Created by physi on 18/12/2024.
//

#ifndef RENDERER_H
#define RENDERER_H
#include "CubeRenderer.h"
#include "../Chunks/Chunk.h"

namespace BlockGame {

    class Renderer {
    public:
        Renderer();
        ~Renderer();

        void RenderChunk(const Chunk* chunk, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

        CubeRenderer* GetCubeRenderer() { return &m_CubeRenderer; }

    private:
        CubeRenderer m_CubeRenderer;
    };

} // BlockGame

#endif //RENDERER_H
