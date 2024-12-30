//
// Created by physi on 17/12/2024.
//

#include "IndexBuffer.h"

namespace OpenGLClasses {
    IndexBuffer::IndexBuffer()
    {
        m_BufferID = 0;
        m_Type = GL_ELEMENT_ARRAY_BUFFER;

        glGenBuffers(1, &m_BufferID);
        Bind();
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_BufferID);
        Unbind();
    }

    void IndexBuffer::BufferData(GLsizeiptr size, void *data, GLenum usage)
    {
        Bind();
        glBufferData(m_Type, size, data, usage);
    }

    void IndexBuffer::Bind()
    {
        glBindBuffer(m_Type, m_BufferID);
    }

    void IndexBuffer::Unbind()
    {
        glBindBuffer(m_Type, 0);
        glBindVertexArray(0);
    }
} // OpenGLClasses