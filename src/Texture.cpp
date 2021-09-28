#include <iostream>
#include "Texture.h"

Texture::Texture(const tinygltf::Image& tex) { 
	glGenTextures(1, &mHandle); 	
	std::cerr << "Handle = " << mHandle << "\n";/*mTexture = tex;*/ }
Texture::Texture() {}
Texture::~Texture() { glDeleteTextures(1, &mHandle); }
void Texture::Bind(TextureUnit unit) const
{
	glActiveTexture(GL_TEXTURE0 + static_cast<unsigned int>(unit));
	glBindTexture(GL_TEXTURE_2D, mHandle);
}
