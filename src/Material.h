#pragma once
#include <vector>
#include <memory>
#include "Color.h"
#include "Resource.h"

class Material
{
public:
	Material() {};
	void SetDiffuseColor(std::vector<double>& c);
	void SetDiffuseTex(std::shared_ptr<Resource>& tex);
	void SetSpecularTex(std::shared_ptr<Resource>& tex);
	void SetNormalTex(std::shared_ptr<Resource>& tex);
	void SetActive();
private:
	//diffuse color
	Color mDiffuseColor;
	std::shared_ptr<Resource> mDiffuseTex;
	std::shared_ptr<Resource> mSpecularTex;
	std::shared_ptr<Resource> mNormalTex;
};