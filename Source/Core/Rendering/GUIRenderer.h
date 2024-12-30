//
// Created by physi on 27/12/2024.
//

#ifndef GUIRENDERER_H
#define GUIRENDERER_H
#include <glm/vec2.hpp>

#include "../Chunks/Chunk.h"
#include "../OpenGL/IndexBuffer.h"
#include "../OpenGL/Texture.h"
#include "../OpenGL/VertexArray.h"
#include "../OpenGL/VertexBuffer.h"
#include "../Player/GUICamera.h"
#include "../OpenGL/Shader.h"

namespace BlockGame {

    class GUIRenderer {
    public:
        GUIRenderer();
        ~GUIRenderer();

        void RenderQuad(const glm::vec2& position, Texture* texture, GUICamera* guiCamera, const float zPos = 0.0f);
        void RenderQuad(const glm::vec2& position, Texture* texture, GUICamera* guiCamera, int w, int h, const float zPos = 0.0f);

    private:
        VertexBuffer m_VBO;
        VertexArray m_VAO;
        IndexBuffer m_IBO;
        Shader m_DefaultShader;
    };

} // BlockGame

#endif //GUIRENDERER_H
