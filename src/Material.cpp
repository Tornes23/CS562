#include "Material.h"

void Material::SetDiffuseColor(std::vector<double>& c) { mDiffuseColor.SetColor(c); }

void Material::SetDiffuseTex(std::shared_ptr<Resource>& tex) { if (tex != nullptr) mDiffuseTex = tex; }
void Material::SetSpecularTex(std::shared_ptr<Resource>& tex) { if (tex != nullptr) mSpecularTex = tex; }
void Material::SetNormalTex(std::shared_ptr<Resource>& tex) { if (tex != nullptr) mNormalTex = tex; }
