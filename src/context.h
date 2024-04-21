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
    void ProcessInput(GLFWwindow* wnd);
    void MouseMove(double x, double y);
    void MouseButton(int button, int action, double x, double y);
    void Reshape(const int width, const int height);

private:
    Context() {}

    bool Init();
    inline bool IsKeyPressed(GLFWwindow* wnd, int key);

    void DrawCude();

    ProgramUPtr m_program;

    VertexLayoutUPtr m_vao;
    BufferUPtr m_vbo;
    BufferUPtr m_ebo;

    TextureUPtr m_texture;
    TextureUPtr m_texture_face;

    // camera parameter
    glm::vec3 m_cameraPos {glm::vec3(0.0f, 0.0f, 3.0f)};
    glm::vec3 m_cameraFront {glm::vec3(0.0f, 0.0f, -1.0f)};
    glm::vec3 m_cameraUp {glm::vec3(0.0f, 1.0f, 0.0f)};
    glm::vec2 m_prevMousePos {glm::vec2(0.0f)};
    bool m_canCameraControl {false};
    float m_cameraPitch {0.0};
    float m_cameraYaw {0.0};


    int m_width{800};
    int m_height{600};

    glm::vec4 m_clearColor {glm::vec4(0.1f, 0.2f, 0.3f, 0.0f)};

};

#endif // __CONTEXT_H__