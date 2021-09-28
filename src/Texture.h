#pragma once
#include "gl/glew.h"
#include "tinyglft/tiny_gltf.h"

struct Texture
{
    enum class TextureUnit {
        None, Diffuse, Normal, Specular, Shadow
    };

    Texture(const tinygltf::Image& tex);
    Texture();
    ~Texture();

    void Bind(TextureUnit _unit = TextureUnit::Diffuse) const;

    GLuint          mHandle;
    //tinygltf::Image mTexture;
};

