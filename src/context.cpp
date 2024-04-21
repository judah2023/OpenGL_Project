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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //====================================================================================================
    
    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    m_cameraFront = 
        glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

    float fov = 45.0f;
    float aspect = m_width / (float)m_height;
    float zNear = 0.01f;
    float zFar = 100.0f;
    auto projection = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    
    // // 1
    // float angle_Camera = glfwGetTime() * glm::pi<float>() * 0.5f;
    // float x = sinf(angle_Camera) * 10.0f;
    // float z = cosf(angle_Camera) * 10.0f;
    // auto cameraPos = glm::vec3(x, 0.0f, z);
    // auto cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    // auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    // auto cameraZ = glm::normalize(cameraPos - cameraTarget);
    // auto cameraX = glm::normalize(glm::cross(cameraUp, cameraZ));
    // auto cameraY = glm::cross(cameraZ, cameraX);
    // auto cameraMat = glm::mat4(
    //     glm::vec4(cameraX,0.0f),
    //     glm::vec4(cameraY,0.0f),
    //     glm::vec4(cameraZ,0.0f),
    //     glm::vec4(cameraPos,0.0f)
    // );
    // auto view = glm::inverse(cameraMat);

    // 2
    auto view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

    for (size_t i = 0; i < cubePositions.size(); i++)
    {
        auto& pos = cubePositions[i];
        float angle = glfwGetTime() * 120.0f + 20.0f * i;
        
        // 1
        // auto trans = glm::translate(glm::mat4(1.0f), pos);
        // auto rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.5f, 0.0f));
        // auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
        // auto model = trans * rot * scale;

        // 2
        auto model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.5f, 0.0f));

        auto transMat = projection * view * model;
        m_program->SetUniform("transMat", transMat);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    }

    //====================================================================================================

}

void Context::ProcessInput(GLFWwindow *wnd)
{
    if (!m_canCameraControl)
        return;

    const float cameraSpeed = 0.05f;
    if (IsKeyPressed(wnd, GLFW_KEY_W))
        m_cameraPos += cameraSpeed * m_cameraFront;
    if (IsKeyPressed(wnd, GLFW_KEY_S))
        m_cameraPos -= cameraSpeed * m_cameraFront;

    auto cameraRight = glm::normalize(glm::cross(m_cameraUp, -m_cameraFront));
    if (IsKeyPressed(wnd, GLFW_KEY_D))
        m_cameraPos += cameraSpeed * cameraRight;
    if (IsKeyPressed(wnd, GLFW_KEY_A))
        m_cameraPos -= cameraSpeed * cameraRight;

    auto cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));
    if (IsKeyPressed(wnd, GLFW_KEY_E))
        m_cameraPos += cameraSpeed * cameraUp;
    if (IsKeyPressed(wnd, GLFW_KEY_Q))
        m_cameraPos -= cameraSpeed * cameraUp;

}

void Context::MouseMove(double x, double y)
{
    if (!m_canCameraControl)
        return;

    auto pos = glm::vec2(x, y);
    auto deltaPos = pos - m_prevMousePos;

    const float cameraRotSpeed = 0.8f;
    m_cameraYaw = glm::clamp(m_cameraYaw - deltaPos.x * cameraRotSpeed, -360.0f, 360.0f);
    m_cameraPitch = glm::clamp(m_cameraPitch - deltaPos.y * cameraRotSpeed, -89.0f, 89.0f);

    m_prevMousePos = pos;
}

void Context::MouseButton(int button, int action, double x, double y)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            m_prevMousePos = glm::vec2(x, y);
            m_canCameraControl = true;
        }
        else if (action == GLFW_RELEASE)
        {
            m_canCameraControl = false;
        }
    }
}

void Context::Reshape(const int width, const int height)
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
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
    /* 
    [  x     y      z             s     t       ]
    */
    // Cube's faces
    -0.5f, -0.5f, -0.5f,        0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,        1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,        1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,        0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,        0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,        1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,        1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,        0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,        1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,        1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,        0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,        0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,        1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,        1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,        0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,        0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,        0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,        1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,        1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,        0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,        0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,        1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,        1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,        0.0f, 0.0f,
    };

    uint32_t indices[] = {
         0,  2,  1,  2,  0,  3,
         4,  5,  6,  6,  7,  4,
         8,  9, 10, 10, 11,  8,
        12, 14, 13, 14, 12, 15,
        16, 17, 18, 18, 19, 16,
        20, 22, 21, 22, 20, 23,
    };

    m_vao = VertexLayout::Create();
    m_vbo = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(vertices));

    m_vao->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nullptr);
    m_vao->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

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
    m_program->SetUniform("tex", 0);
    m_program->SetUniform("tex_face", 1);

    return true;
}

bool Context::IsKeyPressed(GLFWwindow *wnd, int key)
{
    return glfwGetKey(wnd, key) == GLFW_PRESS;
}
