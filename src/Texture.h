#pragma once
#include "gl/glew.h"

namespace tinygltf
{
    struct Image;
}

struct Texture
{
    enum class TextureUnit {
        None, Diffuse, Normal, Specular, Shadow
    };

    Texture(const tinygltf::Image* tex);
    Texture();
    ~Texture();

    void Bind(TextureUnit _unit = TextureUnit::Diffuse) const;

    GLuint          mHandle;
};

