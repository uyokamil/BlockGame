//
// Created by physi on 16/12/2024.
//

#include "OpenGL.h"
#include "VertexArray.h"

namespace OpenGLClasses {
    VertexArray::VertexArray()
    {
        m_ArrayId = 0;
        glGenVertexArrays(1, &m_ArrayId);
        Bind();
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_ArrayId);
        Unbind();
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_ArrayId);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }
} // OpenGLClasses