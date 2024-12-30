//
// Created by physi on 17/12/2024.
//

#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <unordered_map>
#include <glm/fwd.hpp>

#include "OpenGL.h"

namespace OpenGLClasses {

    using namespace std;

    class Shader {
    public:
        Shader(const string &vertexShaderPath, const string &fragmentShaderPath)
        {
            CreateShaderProgram(vertexShaderPath, fragmentShaderPath);
        }

        Shader() {}
        ~Shader();

        void CompileShaders();
        void CreateShaderProgram(const string &vertexShaderPath, const string &fragmentShaderPath);
        GLuint GetShaderProgram() const;

        inline void Use()
        {
            if (m_CompiledShader == false)
            {
                CompileShaders();
            }

            glUseProgram(m_ShaderProgram);
        }

        void SetFloat(const GLchar* paramName, GLfloat value, GLboolean useShader = GL_FALSE);
        void SetInteger(const GLchar* paramName, GLint value, GLboolean useShader = GL_FALSE);
        void SetVector2f(const GLchar* paramName, GLfloat x, GLfloat y, GLboolean useShader = GL_FALSE);
        void SetVector2f(const GLchar* paramName, const glm::vec2& value, GLboolean useShader = GL_FALSE);
        void SetVector3f(const GLchar* paramName, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = GL_FALSE);
        void SetVector3f(const GLchar* paramName, const glm::vec3& value, GLboolean useShader = GL_FALSE);
        void SetVector4f(const GLchar* paramName, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = GL_FALSE);
        void SetVector4f(const GLchar* paramName, const glm::vec4& value, GLboolean useShader = GL_FALSE);
        void SetMatrix4(const GLchar* paramName, const glm::mat4& matrix, GLboolean useShader = GL_FALSE);
        void SetIntegerArray(const GLchar* paramName, const GLint* value, GLsizei count, GLboolean useShader = GL_FALSE);

    private:
        unordered_map<string, GLint> m_LocationMap;
        GLuint m_ShaderProgram;
        bool m_CompiledShader = false;

        GLint GetUniformLocation(string uniformName);
        string m_VertexData;
        string m_VertexPath;
        string m_FragmentData;
        string m_FragmentPath;
    };

} // OpenGLClasses

#endif //SHADER_H
