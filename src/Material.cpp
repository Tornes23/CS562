#include <iostream>
#include "RenderManager.h"
#include "Material.h"

void Material::SetDiffuseColor(std::vector<double>& c) { mDiffuseColor.SetColor(c); }

void Material::SetDiffuseTex(std::shared_ptr<Resource>& tex) { if (tex != nullptr) mDiffuseTex = std::dynamic_pointer_cast<TResource<Texture>>(tex); }
void Material::SetSpecularTex(std::shared_ptr<Resource>& tex) { if (tex != nullptr) mSpecularTex = std::dynamic_pointer_cast<TResource<Texture>>(tex); }
void Material::SetNormalTex(std::shared_ptr<Resource>& tex) { if (tex != nullptr) mNormalTex = std::dynamic_pointer_cast<TResource<Texture>>(tex); }

void Material::SetActive()
{
	//set the texture uniforms and stuff for rendering
	mDiffuseTex->Get()->Bind(Texture::TextureIndex::Diffuse);
	mNormalTex->Get()->Bind(Texture::TextureIndex::Normal);
	mSpecularTex->Get()->Bind(Texture::TextureIndex::Specular);
}
