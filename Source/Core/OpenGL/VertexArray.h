//
// Created by physi on 16/12/2024.
//

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H
#include <GL/gl.h>

namespace OpenGLClasses {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

    private:
        GLuint m_ArrayId;
        GLenum m_Type;
    };

} // OpenGLClasses

#endif //VERTEXARRAY_H
