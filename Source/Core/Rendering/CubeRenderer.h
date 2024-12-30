//
// Created by physi on 17/12/2024.
//

#ifndef CUBERENDERER_H
#define CUBERENDERER_H
#include <vector>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../OpenGL/IndexBuffer.h"
#include "../OpenGL/Shader.h"
#include "../OpenGL/Texture.h"
#include "../OpenGL/VertexArray.h"
#include "../OpenGL/VertexBuffer.h"

namespace BlockGame {
    struct Block;
    enum BlockFaceType : uint8_t;

    using namespace OpenGLClasses;

    struct Cube {
        float sizeX, sizeY, sizeZ;
    };

    class CubeRenderer {
    public:
        CubeRenderer();
        ~CubeRenderer();

        void RenderCube(const Block& block, const glm::vec3& position, const std::vector<BlockFaceType>& visibleFaces, float rotation, const glm::mat4& view, const glm::mat4& projection);

        void RenderOutline(const glm::vec3& position, const glm::mat4& view, const glm::mat4& projection, float scale = 1.0f);

        static glm::vec2 DetermineFaceTexture(int face, const std::vector<glm::vec2>& faceTextures);

    private:
        VertexBuffer m_VBO;
        IndexBuffer m_IBO;
        VertexArray m_VAO;

        Shader m_DefaultShader;
        Shader m_OutlineShader;
    };

} // BlockGame

#endif //CUBERENDERER_H
