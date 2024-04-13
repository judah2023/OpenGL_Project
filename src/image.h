#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "common.h"

CLASS_PTR(Image)
class Image
{
public:
    ~Image();

    static ImageUPtr Load(const std::string& filePath);
    static ImageUPtr Load(int width, int height, int channelCount = 4);

    const uint8_t* GetData() const { return m_data; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetChannelCount() const { return m_channelCount; }

    void SetCheckImage(int gridX, int gridY);

private:
    Image(){}

    bool Load_Internal(const std::string& filePath);
    bool Load_Internal(int width, int height, int channelCount);

    uint8_t* m_data = nullptr;
    int m_width = 0;
    int m_height = 0;
    int m_channelCount = 0;
};

#endif // __IMAGE_H__