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
	if(mDiffuseTex != nullptr)
		mDiffuseTex->Get()->Bind(Texture::TextureIndex::Diffuse);
	if(mNormalTex != nullptr)
		mNormalTex->Get()->Bind(Texture::TextureIndex::Normal);
	if(mSpecularTex!= nullptr)
		mSpecularTex->Get()->Bind(Texture::TextureIndex::Specular);

	//setting the sampler uniforms
	if (mDiffuseTex != nullptr)
		glUniform1i(0, static_cast<int>(Texture::TextureIndex::Diffuse));
	if (mNormalTex != nullptr)
		glUniform1i(1, static_cast<int>(Texture::TextureIndex::Normal));
	if (mSpecularTex != nullptr)
		glUniform1i(2, static_cast<int>(Texture::TextureIndex::Specular));
}
