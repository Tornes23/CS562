#pragma once
#include "gl/glew.h"

namespace tinygltf
{
    struct Image;
}

struct Texture
{
    enum class TextureIndex {
        Diffuse, 
        Normal, 
        Specular, 
    };

    Texture(const tinygltf::Image* tex);
    Texture();
    ~Texture();

    void Bind(TextureIndex _unit = TextureIndex::Diffuse) const;

    GLuint          mHandle;
};

