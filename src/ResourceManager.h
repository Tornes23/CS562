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

	enum class RESOURCE_TYPE
	{
		Texture,
		Model
	};

	std::string GetExtension(const std::filesystem::path& path);
	void Load();
	void LoadFolder(const std::filesystem::path& path);
	void LoadModel(const std::string& file);
	void GetTextures(tinygltf::Model* model);

private:
	std::map<RESOURCE_TYPE, std::map<std::string, std::shared_ptr<Resource>>> mResources;
	ResourceManagerClass() {}
};

#define ResourceManager (ResourceManagerClass::GetInstance())
