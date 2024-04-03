#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"

Image::Image(const char* filename)
{
    if(read(filename))
    {
        printf("Reading %s\n", filename);
        size = w*h*channels;
    }else
    {
        printf("Failed to read %s\n", filename);
    }
}

Image::Image(int w, int h, int channels): w(w), h(h), channels(channels)
{
    size = w*h*channels;
    data = new uint8_t[size];
}

Image::Image(const Image& img) : Image (img.w, img.h, img.channels)
{
    memcpy(data, img.data, img.size);

}
Image::~Image()
{
    stbi_image_free(data);
}

bool Image::read(const char* filename)
{
    data = stbi_load (filename, &w, &h, &channels, 0);
    return data != NULL;
}

bool Image::write(const char* filename)
{
    int success;

    ImageType type = getFileType(filename);
    switch(type)
    {
        case PNG:
            success = stbi_write_png(filename, w, h, channels, data, w*channels);
            break;

        case BMP:
            stbi_write_bmp(filename, w, h, channels, data);
            break;
        
        case JPG:
            stbi_write_jpg(filename, w, h, channels, data, 100);
            break;
    }

    return success != 0;
}

ImageType Image::getFileType(const char* filename)
{
    const char* ext = strrchr(filename, '.');
    if (ext != nullptr)
    {
        if (strcmp(ext, ".png") == 0)
        {
            return PNG;
        }else if (strcmp(ext, ".jpg") == 0)
        {
            return JPG;
        }else if (strcmp(ext, ".bmp") == 0)
        {
            return BMP;
        }
    }
    return PNG;
}

Image& Image::encodeMsg(const char* msg)
{
    //find the size of the message in bits.
    uint32_t len = strlen(msg) * sizeof(char) * 8;
    printf("len = %d\n", len);
    //check if the message fit in the image
    if ((len + HEADER_SIZE) > size)
    {
        printf("message cant fit into the image\n");
        return *this;
    }

    //input the size of the message in bits at the front of the image data
    for (uint8_t i =0; i < HEADER_SIZE; ++i )
    {
        //clear the LSB
        data[i] &= 0xFE;

        //write the size of our message in the image header bit by bit
        data[i] |= ( len >> ( HEADER_SIZE - 1 - i )) & 1UL;
    }

    //input the actual message in bits after the HEADER
    for (uint32_t i =0; i < len; ++i )
    {
        //clear the LSB
        data[i + HEADER_SIZE] &= 0xFE;

        //write the size of our message in the image header bit by bit
        data[i + HEADER_SIZE] |= (msg[i/8] >> ((len - 1 - i) %8)) & 1UL;
    }

    return *this;
}

Image& Image::decodeMsg(char* buffer)
{
    uint32_t len = 0;
    for (uint8_t i = 0; i < HEADER_SIZE; ++i )
    {
        len = (len << 1) | (data[i] & 1);
    }
    printf("encoded len: %d\n", len);
    for(uint32_t i=0; i < len; ++i)
    {
        buffer[i/8] = (buffer[i/8] << 1) | (data[i + HEADER_SIZE] & 1UL);

    }
    return *this;
}
