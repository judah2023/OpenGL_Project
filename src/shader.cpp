#include "shader.h"

Shader::~Shader()
{
    if (m_shader)
    {
        glDeleteShader(m_shader);
    }
}

ShaderUPtr Shader::CreateFromFile(const std::string &fileName, GLenum shaderType)
{
    ShaderUPtr shader(new Shader());
    if (!shader->LoadFile(fileName, shaderType))
        return nullptr;
    return std::move(shader);
}

bool Shader::LoadFile(const std::string &fileName, GLenum shaderType)
{
    auto result = LoadTextFile(fileName);
    if (!result.has_value())
        return false;

    auto& code = result.value();
    const char* codePtr = code.c_str();
    int32_t condeLength = (int32_t)code.length();

    // create and compile shader
    m_shader = glCreateShader(shaderType);
    glShaderSource(m_shader, 1, &codePtr, &condeLength);
    glCompileShader(m_shader);

    // check compile error
    int success = 0;
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[LOG_BUFSIZE];
        glGetShaderInfoLog(m_shader, LOG_BUFSIZE, nullptr, infoLog);
        SPDLOG_INFO("failed to compile shader : \"{}\"", fileName);
        SPDLOG_INFO("reason : {}", infoLog);
        return false;
    }
    return true;
}
