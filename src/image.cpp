#include "image.h"

/* In stb_image.h...
   Do this:
      #define STB_IMAGE_IMPLEMENTATION
   before you include this file in *one* C or C++ file to create the implementation.

*/
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


Image::~Image()
{
    if (m_data)
        stbi_image_free(m_data);
}

ImageUPtr Image::Load(const std::string &filePath)
{
    ImageUPtr image(new Image());
    if (!image->Load_Internal(filePath))
        return nullptr;
    return std::move(image);
}

ImageUPtr Image::Load(int width, int height, int channelCount)
{
    ImageUPtr image(new Image());
    if (!image->Load_Internal(width, height, channelCount))
        return nullptr;
    return std::move(image);
}

void Image::SetCheckImage(int gridX, int gridY)
{
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            int pos = (i* m_width + j) * m_channelCount;
            bool odd = ((j / gridX) + (i / gridY)) & 1;
            uint8_t value = odd ? 0 : 255;
            for (int k = 0; k < m_channelCount; k++)
            {
                m_data[pos + k] = value;
            }

            // not meaning?
            // if (m_channelCount > 3)
            // {
            //     m_data[3] = 255;
            // }
        }
    }
}

bool Image::Load_Internal(const std::string &filePath)
{
    stbi_set_flip_vertically_on_load(true);
    m_data = stbi_load(filePath.c_str(), &m_width, &m_height, &m_channelCount, 0);
    if (!m_data)
    {
        SPDLOG_ERROR("failed to load image : {}", filePath);
        return false;
    }
    return true;
}

bool Image::Load_Internal(int width, int height, int channelCount)
{
    m_width = width;
    m_height = height;
    m_channelCount = channelCount;
    m_data = new uint8_t[width * height * channelCount];
    return m_data ? true : false;
}
