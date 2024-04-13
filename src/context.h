#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "vertex_layout.h"
#include "buffer.h"
#include "texture.h"

CLASS_PTR(Context)
class Context
{
public:
    ~Context();

    static ContextUPtr Create();
    void Render();

private:
    Context() {}

    bool Init();
    
    ProgramUPtr m_program;

    VertexLayoutUPtr m_vao;
    BufferUPtr m_vbo;
    BufferUPtr m_ebo;

    TextureUPtr m_texture;
    TextureUPtr m_texture_face;

};

#endif // __CONTEXT_H__