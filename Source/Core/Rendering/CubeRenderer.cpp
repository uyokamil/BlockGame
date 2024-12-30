//
// Created by physi on 17/12/2024.
//

#include "CubeRenderer.h"

#include <format>

#include "TextureManager.h"
#include "../Utils/Defaults.h"
#include "../Blocks/Block.h"
#include "../Utils/Logger.h"

namespace BlockGame {

    // Cube vertices array (Position + UV)
    static float CubeVertices[] = {
        // NORTH (-Z) - back
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,

        // SOUTH (+Z) - front
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        // WEST (-X) - left
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

        // EAST (+X) - right
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

        // BOTTOM (-Y) - bottom
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // TOP (+Y) - top
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    };

    CubeRenderer::CubeRenderer() : m_VBO(GL_ARRAY_BUFFER)
    {
        m_DefaultShader.CreateShaderProgram("Shaders/BlockVertexShader.glsl", "Shaders/BlockFragmentShader.glsl");
        m_DefaultShader.CompileShaders();

        m_OutlineShader.CreateShaderProgram("Shaders/BlockOutlineVertexShader.glsl", "Shaders/BlockOutlineFragmentShader.glsl");
        m_OutlineShader.CompileShaders();

        m_VAO.Bind();

        m_VBO.BufferData(sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
        m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
        m_VBO.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        m_VAO.Unbind();
    }

    CubeRenderer::~CubeRenderer() = default;

    void CubeRenderer::RenderCube(const Block& block, const glm::vec3& position, const std::vector<BlockFaceType>& visibleFaces, float rotation, const glm::mat4& view, const glm::mat4& projection)
    {
        Shader* useShader = &m_DefaultShader;

        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

        useShader->Use();
        useShader->SetMatrix4("u_Model", model);
        useShader->SetMatrix4("u_View", view);
        useShader->SetMatrix4("u_Projection", projection);
        //useShader->SetInteger("u_Texture", 1);

        m_VAO.Bind();

        for (const auto& face : visibleFaces)
        {
            const Texture* faceTexture = TextureManager::GetTexture(block.GetTextureKey(face));
            faceTexture->Bind(1);
            useShader->SetInteger("u_Texture", 1);

            glDrawArrays(GL_TRIANGLES, face * 6, 6); // Each face has 6 vertices
        }

        m_VAO.Unbind();
    }

    void CubeRenderer::RenderOutline(const glm::vec3 &position, const glm::mat4 &view, const glm::mat4 &projection, float scale)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        model = glm::scale(model, glm::vec3(scale));

        glm::mat4 mvp = projection * view * model;

        m_OutlineShader.Use();
        m_OutlineShader.SetMatrix4("u_Model", model);
        m_OutlineShader.SetMatrix4("u_View", view);
        m_OutlineShader.SetMatrix4("u_Projection", projection);
        m_OutlineShader.SetVector3f("u_Color", glm::vec3(1.0f, 1.f, 0.0f));

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // Enable wireframe mode
        glDisable(GL_CULL_FACE);                              // Disable face culling for better visualization

        m_VAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);         // Render the cube outline
        m_VAO.Unbind();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
    }

    glm::vec2 CubeRenderer::DetermineFaceTexture(int face, const std::vector<glm::vec2>& faceTextures)
    {
        switch (faceTextures.size())
        {
            case 1:  // All faces
                return faceTextures[0];
            case 2:  // Top/Bottom, Sides
                if (face == BlockFaceType::Top || face == BlockFaceType::Bottom) return faceTextures[1];
            return faceTextures[0];
            case 3:  // Sides, Top, Bottom
                if (face == BlockFaceType::Top) return faceTextures[1];
            if (face == BlockFaceType::Bottom) return faceTextures[2];
            return faceTextures[0];
            case 4:  // Full definition
                return faceTextures[face];
            default:
                return {-1.0f, -1.0f};  // Invalid case
        }
    }
} // BlockGame