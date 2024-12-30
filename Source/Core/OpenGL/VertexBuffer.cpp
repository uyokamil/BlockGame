//
// Created by physi on 16/12/2024.
//

#include "VertexBuffer.h"

namespace OpenGLClasses {
    VertexBuffer::VertexBuffer(GLenum type)
    {
        m_BufferID = 0;
        m_Type = type;
        glGenBuffers(1, &m_BufferID);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_BufferID);
        Unbind();
    }

    void VertexBuffer::BufferData(GLsizeiptr size, void *data, GLenum usage) const
    {
        Bind();
        glBufferData(m_Type, size, data, usage);
    }

    void VertexBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, void *data) const
    {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(m_Type, m_BufferID);
    }

    void VertexBuffer::Unbind() const
    {
        glBindBuffer(m_Type, 0);
    }

    void VertexBuffer::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
        const GLvoid *pointer) const
    {
        Bind();
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        glEnableVertexAttribArray(index);
    }

    void VertexBuffer::VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) const
    {
        Bind();
        glVertexAttribIPointer(index, size, type, stride, pointer);
        glEnableVertexAttribArray(index);
    }
} // OpenGLClasses