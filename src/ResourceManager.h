#pragma once
#include <filesystem>
#include <map>
#include "Resource.h"

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
	void Update();
	void LoadFolder(const std::filesystem::path& path);
	void LoadModel(const std::string& file);
	void GetTextures(tinygltf::Model* model);
	void GetMaterials(tinygltf::Model* model);

	template<typename T>
	T* GetResource(const std::string& name);

private:
	std::map<std::string, std::map<std::string, std::shared_ptr<Resource>>> mResources;
	ResourceManagerClass() {}
};

#define ResourceManager (ResourceManagerClass::GetInstance())

template<typename T>
inline T* ResourceManagerClass::GetResource(const std::string& name)
{
	auto map =  mResources.at(typeid(T).name());
	TResource<T>* res = dynamic_cast<TResource<T>*>(map.at(name).get());
	if (res) return (res->get());
	return nullptr;
}
