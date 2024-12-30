//
// Created by physi on 17/12/2024.
//

#include "Texture.h"

#include "../Utils/Logger.h"

namespace OpenGLClasses {
    Texture::Texture(string path, bool flipped, GLenum type, GLenum textureWrapU,
        GLenum textureWrapV, array<GLfloat, 8> texCoords, bool clean)
    {
        m_Texture = 0;
        CreateTexture(path, flipped, type, textureWrapU, textureWrapV, texCoords, clean);
    }

    Texture::Texture()
    {
        m_Texture = 0;
        m_TexCoords = { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
        m_ShouldClean = true;
        m_Width = 0;
        m_Height = 0;
        m_Type = GL_TEXTURE_2D;
        m_IntFormat = GL_RGBA;
        m_Channels = 0;
        deleteTexture = true;
    }

    Texture::~Texture()
    {
        if (deleteTexture == 1 && m_Texture != 0)
        {
            glDeleteTextures(1, &m_Texture);
            m_Texture = 0;
        }
    }

    void Texture::CreateTexture(string path, bool flipped, GLenum type,
        GLenum textureWrapU, GLenum textureWrapV, array<GLfloat, 8> texCoords, bool clean)
    {
        // Flip image vertically if needed (it's needed)
        stbi_set_flip_vertically_on_load(flipped);

        GLenum internalFormat = GL_RGBA;

        deleteTexture = true;
        m_Texture = 0;
        m_TexCoords = texCoords;
        m_ShouldClean = clean;
        m_Type = type;
        m_Path = path;

        // Generate and bind texture
        glGenTextures(1, &m_Texture);
        glBindTexture(type, m_Texture);

        // Set texture wrapping parameters
        glTexParameteri(type, GL_TEXTURE_WRAP_S, textureWrapU);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, textureWrapV);

        // Set texture filtering parameters
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Load texture image data
        unsigned char* image = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 4);

        if (!image)
        {
            Logger::Error("Failed to load texture at path: " + path);
            return;
        }

        // Determine texture channels format
        if (m_Channels == 1) internalFormat = GL_RED;
        else if (m_Channels == 3) internalFormat = GL_RGB;
        else if (m_Channels == 4) internalFormat = GL_RGBA;

        // Upload texture data to GPU & gen mipmaps
        glTexImage2D(type, 0, internalFormat, m_Width, m_Height, 0, internalFormat, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(type);

        // Free image memory if needed
        if (image && clean)
        {
            stbi_image_free(image);
        }

        Logger::Debug(std::format("Texture::CreateTexture: Texture '{}' loaded. Width: {}, Height: {}", path, m_Width, m_Height));
    }

    void Texture::IntCreateTexture(GLuint id, array<GLfloat, 8> texCoords, int w, int h, bool deleteTexture)
    {
        m_Texture = id;
        m_TexCoords = texCoords;
        m_Width = w;
        m_Height = h;
        this->deleteTexture = deleteTexture;
    }

    void Texture::Bind(int slot) const
    {
        // Activate texture slot with ID
        glActiveTexture(GL_TEXTURE0 + slot);

        // Bind texture slot
        glBindTexture(m_Type, m_Texture);
    }

    void Texture::Unbind() const
    {
        // Unbind any active texture of this type
        glBindTexture(m_Type, 0);
    }

    ImageData GetTextureData(const std::string &path)
    {
        ImageData output{};

        output.imageData = stbi_load(path.c_str(), &output.width, &output.height, &output.channels, 4);
        return output;
    }
} // OpenGLClasses