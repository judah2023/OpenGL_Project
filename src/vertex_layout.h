#ifndef __VERTEX_LAYOUT_H__
#define __VERTEX_LAYOUT_H__

#include "common.h"

CLASS_PTR(VertexLayout)
class VertexLayout
{
public:
    ~VertexLayout();

    static VertexLayoutUPtr Create();

    uint32_t Get() const { return m_vao; }
    void Bind() const;
    void SetAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * offset) const;
    void DisableAttrib(GLuint index) const;

private:
    VertexLayout() {}
    void Init();

    uint32_t m_vao = 0;
};

#endif // __VERTEX_LAYOUT_H__