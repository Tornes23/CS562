#pragma once
#include <filesystem>
#include <map>
#include "tinyglft/stb_image.h"
#include "Resource.h"
#include "Utils.h"

namespace tinygltf {
	class Model;
}

class ResourceManagerClass
{
public:
	//singleton stuff
	ResourceManagerClass(ResourceManagerClass const&) = delete;
	void operator=(ResourceManagerClass const&) = delete;
	static ResourceManagerClass& GetInstance()
	{
		static ResourceManagerClass instance;
		return instance;
	}


	std::string GetExtension(const std::filesystem::path& path);
	void Load(bool reload = false);
	void LoadScene(const std::string& scene);
	void Update();
	void LoadFolder(const std::filesystem::path& path);
	void LoadModel(const std::string& file);
	void LoadTexture(const std::string& file);
	void GetTextures(tinygltf::Model* model);
	void LoadMaterials(tinygltf::Model* model);
	#undef LoadImage
	stbi_uc* LoadImage(const std::string& file, int* width, int* height, int* channels, int desired_channels);

	template<typename T>
	T* GetResource(const std::string& name);

private:
	std::map<std::string, std::map<std::string, std::shared_ptr<Resource>>> mResources;
	ResourceManagerClass() {}
	std::string mSceneName;
};

#define ResourceManager (ResourceManagerClass::GetInstance())

template<typename T>
inline T* ResourceManagerClass::GetResource(const std::string& name)
{
	auto map =  mResources.at(Utils::GetTypeName<T>());
	TResource<T>* res = dynamic_cast<TResource<T>*>(map.at(name).get());
	if (res) return (res->Get());
	return nullptr;
}
