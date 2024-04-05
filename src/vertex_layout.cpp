#include "vertex_layout.h"

VertexLayout::~VertexLayout()
{
    if (m_vao)
        glDeleteVertexArrays(1, &m_vao);
}

VertexLayoutUPtr VertexLayout::Create()
{
    VertexLayoutUPtr layout(new VertexLayout());
    layout->Init();
    return std::move(layout);
}

void VertexLayout::Bind() const
{
    glBindVertexArray(m_vao);
}

void VertexLayout::SetAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *offset) const
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, stride, offset);
}

void VertexLayout::DisableAttrib(GLuint index) const
{
    
}

void VertexLayout::Init()
{
    glGenVertexArrays(1, &m_vao);
    Bind();
}
