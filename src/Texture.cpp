#include <iostream>
#include "Texture.h"
#include "tinyglft/tiny_gltf.h"

Texture::Texture(const tinygltf::Image* tex) 
{ 
	glGenTextures(1, &mHandle); 	

    glBindTexture(GL_TEXTURE_2D, mHandle);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum format = GL_RGBA;

    if (tex->component == 1)
        format = GL_RED;
    else if (tex->component == 2)
        format = GL_RG;
    else if (tex->component == 3)
        format = GL_RGB;
    else
        format = GL_RGBA;

    GLenum type = GL_UNSIGNED_BYTE;
    if (tex->bits == 16)
        type = GL_UNSIGNED_SHORT;

    glTexImage2D(GL_TEXTURE_2D, 0, format, tex->width, tex->height, 0,
        format, type, &tex->image.at(0));
}
Texture::Texture() {}
Texture::~Texture() { glDeleteTextures(1, &mHandle); }

void Texture::Bind(TextureIndex unit) const
{
	glActiveTexture(GL_TEXTURE0 + static_cast<unsigned int>(unit));
	glBindTexture(GL_TEXTURE_2D, mHandle);
}
