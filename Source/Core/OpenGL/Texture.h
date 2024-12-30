//
// Created by physi on 17/12/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <array>
#include <string>

#include "OpenGL.h"
#include "stb_image.h"

namespace OpenGLClasses {

    using namespace std;

    struct ImageData
    {
        unsigned char* imageData;
        int width, height;
        int channels;
    };

    class Texture {
    public:
        /// Initialize and create a texture
        /// @param path Texture path
        /// @param flipped Flip image vertically
        /// @param type Texture type (e.g. GL_TEXTURE_2D)
        /// @param minFilter Texture filtering setting
        /// @param magFilter Texture filtering setting
        /// @param textureWrapU Texture wrap setting for U coordinate
        /// @param textureWrapV Texture wrap setting for V coordinate
        /// @param texCoords Default texture coordinates
        /// @param clean Free loaded image data after usage
        Texture(string path, bool flipped = true, GLenum type = GL_TEXTURE_2D,
            GLenum textureWrapU = GL_REPEAT, GLenum textureWrapV = GL_REPEAT,
            array<GLfloat, 8> texCoords = { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
            bool clean = true);

        Texture();
        ~Texture();

        /// Create a new texture from path
        /// @param path Texture path
        /// @param flipped Flip image vertically
        /// @param type Texture type (e.g. GL_TEXTURE_2D)
        /// @param minFilter Texture filtering setting
        /// @param magFilter Texture filtering setting
        /// @param textureWrapU Texture wrap setting for U coordinate
        /// @param textureWrapV Texture wrap setting for V coordinate
        /// @param texCoords Default texture coordinates
        /// @param clean Free loaded image data after usage
        void CreateTexture(string path, bool flipped = true, GLenum type = GL_TEXTURE_2D,
                           GLenum textureWrapU = GL_REPEAT, GLenum textureWrapV = GL_REPEAT,
                           array<GLfloat, 8> texCoords = { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
                           bool clean = true);

        /// Assign an existing texture ID with dimensions and coords
        /// @param id Texture ID
        /// @param texCoords Texture coordinates
        /// @param w Texture width
        /// @param h Texture height
        /// @param deleteTexture Clean up after usage?
        void IntCreateTexture(GLuint id, array<GLfloat, 8> texCoords, int w, int h, bool deleteTexture);

        /// Get Image Width
        inline int GetWidth() const { return m_Width;}

        /// Get Image Height
        inline int GetHeight() const { return m_Height;}

        /// Get Texture ID
        inline GLuint GetTextureID() const { return m_Texture; }

        /// Get Texture Path
        inline string GetTexturePath() const { return m_Path; }

        /// Get Texture Coordinates
        array<GLfloat, 8> GetTexCoords() const { return m_TexCoords; }

        /// Bind Texture
        void Bind(int slot = 0) const;

        /// Unbind Texture
        void Unbind() const;

        int deleteTexture;

        GLuint m_Texture;

    private:
        bool m_ShouldClean;

        int m_Width, m_Height;
        int m_Channels;
        GLenum m_IntFormat;

        GLenum m_Type;
        string m_Path;
        array<GLfloat, 8> m_TexCoords;
    };

    ImageData GetTextureData(const std::string& path);

} // OpenGLClasses

#endif //TEXTURE_H
