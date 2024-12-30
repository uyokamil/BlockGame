//
// Created by physi on 17/12/2024.
//

#include "Shader.h"

#include <chrono>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

#include "../Utils/Logger.h"

namespace OpenGLClasses {
    Shader::~Shader()
    {
        glUseProgram(0);
    }

    void Shader::CompileShaders()
    {
        // Mark the shader as uncompiled as we are starting a new compilation.
        m_CompiledShader = false;

        // Start timing shader compilation.
        const auto start = chrono::steady_clock::now();

        GLint success; // Success flag.
        GLchar GLInfoLog[512]; // Buffer for shader error messages.

        // Create empty shader objects for vert and frag shaders.
        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Convert string source code to c-style strings for OpenGL.
        const char* vertShaderChar = m_VertexData.c_str();
        const char* fragShaderChar = m_FragmentData.c_str();

        // Attach source code to shaders and compile them.
        glShaderSource(vertShader, 1, &vertShaderChar, nullptr);
        glShaderSource(fragShader, 1, &fragShaderChar, nullptr);
        glCompileShader(vertShader);

        // Check for vertex shader compilation errors
        glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertShader, 512, nullptr, GLInfoLog);
            Logger::Error(std::format("Vertex shader compilation error: {}", GLInfoLog));
            return; // Early exit on failure
        }

        // Compile the fragment shader
        glCompileShader(fragShader);

        // Check fragment shader compilation errors
        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragShader, 512, nullptr, GLInfoLog);
            Logger::Error(std::format("Fragment shader compilation error: {}", GLInfoLog));
            return; // Early exit on failure
        }

        // Create a shader program and attach the compiled shaders
        m_ShaderProgram = glCreateProgram();
        glAttachShader(m_ShaderProgram, vertShader);
        glAttachShader(m_ShaderProgram, fragShader);
        glLinkProgram(m_ShaderProgram);

        // Check for any linking errors
        glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_ShaderProgram, 512, nullptr, GLInfoLog);
            Logger::Error(std::format("Shader program link error: {}", GLInfoLog));
            return; // Early exit on failure
        }

        // Delete shader objects for clean up (not needed after linking)
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        // Mark shader as compiled on success
        m_CompiledShader = true;

        // Complete timing calculation & print log
        const auto end = chrono::steady_clock::now();
        double elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        Logger::Debug(std::format("Successfully compiled vertex shader \"{}\", fragment shader \"{}\", and linked program. Elapsed: {}", vertShader, fragShader, elapsed));
    }

    void Shader::CreateShaderProgram(const string &vertexShaderPath, const string &fragmentShaderPath)
    {
        string vertexShaderCode;
        string fragmentShaderCode;

        try
        {
            ifstream vertexShaderFile;
            ifstream fragmentShaderFile;

            // Enable specified exceptions for file I/O
            vertexShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
            fragmentShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

            // Open shader source files
            vertexShaderFile.open(vertexShaderPath, ios::in);
            fragmentShaderFile.open(fragmentShaderPath, ios::in);

            // Ensure files are open
            if (!vertexShaderFile.is_open() || !fragmentShaderFile.is_open())
            {
                throw std::runtime_error("Failed to open shader files.");
            }

            m_VertexPath = vertexShaderPath;
            m_FragmentPath = fragmentShaderPath;

            // Read file contents
            std::stringstream vertexShaderSource, fragmentShaderSource;
            vertexShaderSource << vertexShaderFile.rdbuf();
            fragmentShaderSource << fragmentShaderFile.rdbuf();

            // Store file paths and data
            m_VertexPath = vertexShaderPath;
            m_VertexData = vertexShaderSource.str();

            m_FragmentPath = fragmentShaderPath;
            m_FragmentData = fragmentShaderSource.str();
        }
        catch (std::ifstream::failure& e)
        {
            Logger::Error(std::format("Error reading shader files: {}", e.what()));
        }
        catch (std::exception& e)
        {
            Logger::Error(std::format("General error: {}", e.what()));
        }
    }

    GLuint Shader::GetShaderProgram() const
    {
        return m_ShaderProgram;
    }

    void Shader::SetFloat(const GLchar *paramName, GLfloat value, GLboolean useShader)
    {
        if (useShader) Use();

        glUniform1f(GetUniformLocation(paramName), value);
    }

    void Shader::SetInteger(const GLchar *paramName, GLint value, GLboolean useShader)
    {
        if (useShader) Use();

        glUniform1i(GetUniformLocation(paramName), value);
        GLint uniformLocation = glGetUniformLocation(m_ShaderProgram, paramName);
        if (uniformLocation == -1)
        {
            Logger::Error(std::format("Failed to get uniform location: {}", paramName));
        }
    }

    void Shader::SetVector2f(const GLchar *paramName, GLfloat x, GLfloat y, GLboolean useShader)
    {
        if (useShader) Use();

        glUniform2f(GetUniformLocation(paramName), x, y);
    }

    void Shader::SetVector2f(const GLchar *paramName, const glm::vec2 &value, GLboolean useShader)
    {
        if (useShader) Use();

        glUniform2f(GetUniformLocation(paramName), value.x, value.y);
    }

    void Shader::SetVector3f(const GLchar *paramName, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
    {
        if (useShader) Use();

        glUniform3f(GetUniformLocation(paramName), x, y, z);
    }

    void Shader::SetVector3f(const GLchar *paramName, const glm::vec3 &value, GLboolean useShader)
    {
        if (useShader) Use();

        glUniform3f(GetUniformLocation(paramName), value.x, value.y, value.z);
    }

    void Shader::SetVector4f(const GLchar *paramName, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
    {
        if (useShader) Use();

        glUniform4f(GetUniformLocation(paramName), x, y, z, w);
    }

    void Shader::SetVector4f(const GLchar *paramName, const glm::vec4 &value, GLboolean useShader)
    {
        if (useShader) Use();

        glUniform4f(GetUniformLocation(paramName), value.x, value.y, value.z, value.w);
    }

    void Shader::SetMatrix4(const GLchar *paramName, const glm::mat4 &matrix, GLboolean useShader)
    {
        if (useShader) Use();

        glUniformMatrix4fv(GetUniformLocation(paramName), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::SetIntegerArray(const GLchar *paramName, const GLint *value, GLsizei count, GLboolean useShader)
    {
        if (useShader) Use();

        glUniform1iv(GetUniformLocation(paramName), count, value);
    }

    GLint Shader::GetUniformLocation(string uniformName)
    {
        if (!m_LocationMap.contains(uniformName))
        {
            const GLint loc = glGetUniformLocation(m_ShaderProgram, uniformName.c_str());
            if (loc == -1)
            {
                Logger::Error(std::format("[Shader::GetUniformLocation] Uniform {} not found!", uniformName));
            }

            m_LocationMap[uniformName] = loc;
        }
        return m_LocationMap[uniformName];
    }
} // OpenGLClasses