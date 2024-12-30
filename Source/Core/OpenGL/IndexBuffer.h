//
// Created by physi on 17/12/2024.
//

#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "OpenGL.h"

namespace OpenGLClasses {

    class IndexBuffer {
    public:
        IndexBuffer();
        ~IndexBuffer();

        /// <summary>
        /// Uploads index data from the CPU to the GPU using glBufferData
        /// </summary>
        /// <param name="size">Size of the data in bytes</param>
        /// <param name="data">Pointer to the data</param>
        /// <param name="usage">OpenGL usage hint (e.g. GL_STATIC_DRAW, GL_DYNAMIC_DRAW)</param>
        void BufferData(GLsizeiptr size, void* data, GLenum usage);

        void Bind();
        void Unbind();

    private:
        GLuint m_BufferID;
        GLenum m_Type;
    };

} // OpenGLClasses

#endif //INDEXBUFFER_H
