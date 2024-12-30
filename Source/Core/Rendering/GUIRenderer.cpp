//
// Created by physi on 27/12/2024.
//

#include "GUIRenderer.h"

namespace BlockGame {
    GUIRenderer::GUIRenderer() : m_VBO(GL_ARRAY_BUFFER)
    {
        GLuint indexBuffer[6] = { 0, 1, 3, 1, 2, 3 }; // two triangles sharing vertices

        m_VAO.Bind();
        m_VBO.Bind();

        m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)nullptr);   // Vertex positions
        m_VBO.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // Texture coords

        m_IBO.Bind();
        m_IBO.BufferData(sizeof(indexBuffer), indexBuffer, GL_STATIC_DRAW);

        m_VAO.Unbind();

        // Load and compile shader
        m_DefaultShader.CreateShaderProgram("Shaders/GUIVertexShader.glsl", "Shaders/GUIFragmentShader.glsl");
        m_DefaultShader.CompileShaders();
    }

    GUIRenderer::~GUIRenderer() = default;

    void GUIRenderer::RenderQuad(const glm::vec2& position, Texture* texture, GUICamera* guiCamera, const float zPos)
    {
        RenderQuad(position, texture, guiCamera, texture->GetWidth(), texture->GetHeight(), zPos);
    }

    void GUIRenderer::RenderQuad(const glm::vec2& position, Texture* texture, GUICamera* guiCamera, int w,
        int h, const float zPos)
    {
        glDisable(GL_DEPTH_TEST);   // No depth test for 2D rendering
        glDisable(GL_CULL_FACE);    // No face culling for 2D rendering

        // get texture coords
        const std::array<GLfloat, 8> texCoords = texture->GetTexCoords();

        // Calculate vertex positions based positions and dimensions
        float x = position.x;
        float y = position.y;
        float width = x + static_cast<float>(w);
        float height = y + static_cast<float>(h);

        // Define vertex data [x, y, z,  u, v]
        GLfloat vertices[] = {
            width,      y,          zPos,       texCoords[0], texCoords[1],
            width,      height,     zPos,       texCoords[2], texCoords[3],
            x,          height,     zPos,       texCoords[4], texCoords[5],
            x,          y,          zPos,       texCoords[6], texCoords[7],
        };

        // activate shader
        m_DefaultShader.Use();

        texture->Bind(1);

        m_DefaultShader.SetMatrix4("u_Projection", guiCamera->GetProjectionMatrix());
        m_DefaultShader.SetMatrix4("u_View", guiCamera->GetViewMatrix());
        m_DefaultShader.SetMatrix4("u_Model", glm::mat4(1.0f));
        m_DefaultShader.SetInteger("u_Texture", 1);

        // Update buffer data and draw quad
        m_VAO.Bind();
        m_VBO.BufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)nullptr);
        m_VAO.Unbind();

        // restore depth testing
        glEnable(GL_DEPTH_TEST);
    }
} // BlockGame