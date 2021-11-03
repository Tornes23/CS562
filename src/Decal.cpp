#include <gtx/transform.hpp>
#include <gl/glew.h>
#include "Texture.h"
#include "Decal.h"


void Decal::GenM2W()
{
	mM2W = glm::mat4x4(1.0);
	mM2W = glm::translate(mM2W, mPosition);
	mM2W = mM2W * glm::rotate(glm::radians(mRotation.x), glm::vec3(1, 0, 0));
	mM2W = mM2W * glm::rotate(glm::radians(mRotation.y), glm::vec3(0, 1, 0));
	mM2W = mM2W * glm::rotate(glm::radians(mRotation.z), glm::vec3(0, 0, 1));
	mM2W = glm::scale(mScale);
}

void Decal::SetUniforms()
{
	//set the texture uniforms and stuff for rendering
	if (mDiffuse != nullptr)
		mDiffuse->Bind(Texture::TextureIndex::Diffuse);
	if (mNormal!= nullptr)
		mNormal->Bind(Texture::TextureIndex::Normal);
	if (mMetallic != nullptr)
		mMetallic->Bind(Texture::TextureIndex::Specular);

	//setting the sampler uniforms
	if (mDiffuse != nullptr)
		glUniform1i(3, static_cast<int>(Texture::TextureIndex::Diffuse));
	if (mNormal!= nullptr)
		glUniform1i(4, static_cast<int>(Texture::TextureIndex::Normal));
	if (mMetallic != nullptr)
		glUniform1i(5, static_cast<int>(Texture::TextureIndex::Specular));
}
