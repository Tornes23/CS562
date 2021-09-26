#pragma once
#include <filesystem>

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
	void LoadModels();
	void LoadFolder(const std::filesystem::path& path);
private:
	//std::map<RESOURCE_TYPE, std::vector<std::shared_ptr<Resource>>> mResources;
	ResourceManagerClass() {}
};

#define ResourceManager (ResourceManagerClass::GetInstance())
