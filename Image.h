#include <cstdio>
#include <stdint.h>

#define HEADER_SIZE sizeof(uint32_t) * 8

enum ImageType
{
    PNG, JPG, BMP
};

struct Image 
{
    uint8_t* data = NULL;
    size_t size = 0;
    int w, h, channels;

    Image(const char* filename);
    Image(int w, int h, int channels);
    Image(const Image& img);
    ~Image();

    bool read(const char* filename);
    bool write(const char* filename);

    ImageType getFileType(const char* filename);

    Image& encodeMsg(const char* msg);
    Image& decodeMsg(char* buffer);

};