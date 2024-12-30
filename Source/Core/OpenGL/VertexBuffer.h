//
// Created by physi on 16/12/2024.
//

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "OpenGL.h"

namespace OpenGLClasses {

    class VertexBuffer {
    public:
        VertexBuffer(GLenum type);
        ~VertexBuffer();

        /// <summary>
        /// Uploads vertex data to the GPU.
        /// </summary>
        /// <param name="size">Size of the data in bytes</param>
        /// <param name="data">Pointer to vertex data</param>
        /// <param name="usage">Usage hint (e.g. GL_STATIC_DRAW or GL_DYNAMIC_DRAW)</param>
        void BufferData(GLsizeiptr size, void* data, GLenum usage) const;

        /// <summary>
        /// Uploads a specific portion of the buffer to the GPU.
        /// </summary>
        void BufferSubData(GLintptr offset, GLsizeiptr size, void* data) const;

        /// <summary>
        /// Binds & activates the buffer
        /// </summary>
        void Bind() const;

        /// <summary>
        /// Unbinds the buffer
        /// </summary>
        void Unbind() const;

        /// <summary>
        /// Define how vertex data is structured for shaders
        /// </summary>
        /// <param name="index">Attribute location in the shader</param>
        /// <param name="size">Number of components (e.g. 3 for vec3)</param>
        /// <param name="type">GL type (e.g. GL_FLOAT)</param>
        /// <param name="normalized">Should be normalized?</param>
        /// <param name="stride">Stride between attributes</param>
        /// <param name="pointer">Byte offset where this attribute starts in the buffer</param>
        void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) const;

        /// <summary>
        /// Define how vertex data is structured for shaders for integer attributes
        /// </summary>
        /// <param name="index">Attribute location in the shader</param>
        /// <param name="size">Number of components (e.g. 3 for vec3)</param>
        /// <param name="type">GL type (e.g. GL_FLOAT)</param>
        /// <param name="stride">Stride between attributes</param>
        /// <param name="pointer">Byte offset where this attribute starts in the buffer</param>
        void VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer) const;

    private:
        GLuint m_BufferID;
        GLenum m_Type;
    };

} // OpenGLClasses

#endif //VERTEXBUFFER_H
