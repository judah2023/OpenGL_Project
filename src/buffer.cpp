#include "buffer.h"

Buffer::~Buffer()
{
    if (m_buffer)
    {
        glDeleteBuffers(1, &m_buffer);
    }
}

BufferUPtr Buffer::CreateWithData(GLenum bufferType, uint32_t usage, const void *data, size_t dataSize)
{
    BufferUPtr buffer(new Buffer());
    if (!buffer->Init(bufferType, usage, data, dataSize))
        return nullptr;
    return std::move(buffer);
}

void Buffer::Bind() const
{
    glBindBuffer(m_type, m_buffer);
}

bool Buffer::Init(GLenum bufferType, uint32_t usage, const void *data, size_t dataSize)
{
    m_type = bufferType;
    m_usage = usage;
    glGenBuffers(1, &m_buffer);
    Bind();
    glBufferData(m_type, dataSize, data, m_usage);
    return true;    // true 밖에 반환하지 않는데 bool 반환자가 필요한가? (적어도 이 함수안의 동작들은 실패하지 않는 듯 하다)
}
