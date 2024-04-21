#include "program.h"

Program::~Program()
{
    if (m_program)
    {
        glDeleteProgram(m_program);
    }
}

ProgramUPtr Program::Creaete(const std::vector<ShaderPtr> &shaders)
{
    ProgramUPtr program(new Program());
    if (!program->Link(shaders))
        return nullptr;
    return std::move(program);
}

void Program::Use() const
{
    glUseProgram(m_program);
}

void Program::SetUniform(const std::string &name, int value) const
{
    auto location = glGetUniformLocation(m_program, name.c_str());
    glUniform1i(location, value);
}

void Program::SetUniform(const std::string &name, const glm::mat4 &value) const
{
    auto location = glGetUniformLocation(m_program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

bool Program::Link(const std::vector<ShaderPtr> &shaders)
{
    m_program = glCreateProgram();
    for (auto& shader : shaders)
    {
        glAttachShader(m_program, shader->Get());
    }
    glLinkProgram(m_program);

    int success = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[LOG_BUFSIZE];
        glGetProgramInfoLog(m_program, LOG_BUFSIZE, nullptr, infoLog);
        SPDLOG_ERROR("failed to link program : {}", m_program);
        return false;
    }
    return true;
}
