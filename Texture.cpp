#include "Texture.hpp"

#include <SOIL2.h>

#include <vector>
#include <iostream>

namespace
{
    /**
    Ѕиблиотека SOIL читает текстуры перевернутыми
    */
    void invertY(unsigned char* image, int width, int height, int channels)
    {
        for (int j = 0; j * 2 < height; ++j)
        {
            unsigned int index1 = j * width * channels;
            unsigned int index2 = (height - 1 - j) * width * channels;
            for (int i = 0; i < width * channels; i++)
            {
                unsigned char temp = image[index1];
                image[index1] = image[index2];
                image[index2] = temp;
                ++index1;
                ++index2;
            }
        }
    }
}

TexturePtr loadTexture(const std::string& filename, bool gamma, bool withAlpha)
{
    int width, height, channels;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, withAlpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
    if (!image)
    {
        std::cerr << "SOIL loading error: " << SOIL_last_result() << std::endl;
        return 0;
    }

    invertY(image, width, height, withAlpha ? 4 : 3);

    GLint internalFormat = GL_RGB8;
    if (gamma)
    {
        internalFormat = GL_SRGB8;
    }
    else if (withAlpha)
    {
        internalFormat = GL_RGBA8;
    }

    GLint format = withAlpha ? GL_RGBA : GL_RGB;

    TexturePtr texture = std::make_shared<Texture>(GL_TEXTURE_2D);
    texture->setTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, format, GL_UNSIGNED_BYTE, image);
    texture->generateMipmaps();

    SOIL_free_image_data(image);

    return texture;
}