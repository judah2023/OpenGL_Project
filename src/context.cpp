#include "context.h"

Context::~Context()
{
}

ContextUPtr Context::Create()
{
    ContextUPtr context(new Context());
    if (!context->Init())
        return nullptr;

    return std::move(context);
}

void Context::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    /////////////////////////////////////////////////////////////////
    float timer = glfwGetTime() * 5;
    auto u_time = glGetUniformLocation(m_program->Get(), "u_time");
    glUniform1f(u_time, timer);
    /////////////////////////////////////////////////////////////////

    m_program->Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

bool Context::Init()
{
    
    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id : {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id : {}", fragShader->Get());

    m_program = Program::Creaete({fragShader, vertShader});
    if (!m_program)
        return false;
    SPDLOG_INFO("program id : {}", m_program->Get());
    
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

float vertices[] = {
    0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f, // top right, red
    0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f, // bottom right, green
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f, // bottom left, blue
    -0.5f, 0.5f, 0.0f,      1.0f, 1.0f, 0.0f, // top left, yellow
};

    uint32_t indices[] = {
        0, 1, 3,    // first triangle
        1, 2, 3,    // second triangle
    };

    m_vao = VertexLayout::Create();
    m_vbo = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(vertices));

    m_vao->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, nullptr);
    m_vao->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));

    m_ebo = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(indices));

    return true;
}
