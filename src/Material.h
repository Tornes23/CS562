#pragma once
#include <vector>
#include <memory>
#include "Color.h"
#include "Resource.h"
#include "Texture.h"

class Material
{
public:
	enum TexIndex
	{
		Diffuse,
		Normal,
		Specular
	};

	Material() {};
	void SetDiffuseColor(std::vector<double>& c);
	void SetDiffuseTex(std::shared_ptr<Resource>& tex);
	void SetSpecularTex(std::shared_ptr<Resource>& tex);
	void SetNormalTex(std::shared_ptr<Resource>& tex);
	void SetActive();
private:
	//diffuse color
	Color mDiffuseColor;
	std::shared_ptr<TResource<Texture>> mDiffuseTex;
	std::shared_ptr<TResource<Texture>> mSpecularTex;
	std::shared_ptr<TResource<Texture>> mNormalTex;
};