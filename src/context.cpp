#include "context.h"

#include "image.h"

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
    float timer = glfwGetTime() * 3;
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
// [ x     y     z         r     g     b         s     t    ]
    0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,     1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f,     0.0f, 1.0f,
};

    uint32_t indices[] = {
        0, 1, 3,    // first triangle
        1, 2, 3,    // second triangle
    };

    m_vao = VertexLayout::Create();
    m_vbo = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(vertices));

    m_vao->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr);
    m_vao->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
    m_vao->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

    m_ebo = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(indices));

    // image load
    auto image = Image::Load("./image/container.jpg");
    if (!image)
        return false;
    SPDLOG_INFO("image : {} X {}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    // check image create
    // auto image = Image::Load(512, 512);
    // image->SetCheckImage(16, 16);
    
    m_texture = Texture::CreateFromImage(image.get());

    auto image_face = Image::Load("./image/awesomeface.png");
    m_texture_face = Texture::CreateFromImage(image_face.get());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->Get());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture_face->Get());

    m_program->Use();
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 0);
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex_face"), 1);
    return true;
}
