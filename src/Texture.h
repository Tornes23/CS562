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
        Specular
    };

    Texture(const tinygltf::Image* tex);
    Texture(const std::string& file);
    Texture();
    ~Texture();

    void Bind(TextureIndex _unit = TextureIndex::Diffuse) const;
    void Bind(unsigned index) const;

    GLuint          mHandle;
    std::string mName;
};

