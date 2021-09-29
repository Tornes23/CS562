#include <iostream>
#include "ResourceManager.h"
#include "Texture.h"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "Model.h"

std::string ResourceManagerClass::GetExtension(const std::filesystem::path& path)
{
	std::string name = path.string();
	std::string ext = name.substr(name.find_last_of(".") + 1, name.length());

	return ext;
}

void ResourceManagerClass::Load()
{
	LoadFolder("./data/gltf/");
}

void ResourceManagerClass::LoadFolder(const std::filesystem::path& path)
{
	for (auto& it : std::filesystem::directory_iterator(path))
	{
		std::string ext = GetExtension(it.path());
		if (ext.find("/") != ext.npos || ext.find("\\") != ext.npos)
		{
			LoadFolder(it.path());
		}
		else
		{
			for (unsigned i = 0; i < ext.length(); i++)
				ext[i] = toupper(ext[i]);

			if (ext == "GLTF")
				LoadModel(it.path().string());

		}
	}
}

void ResourceManagerClass::LoadModel(const std::string& file)
{
	//load model
	tinygltf::TinyGLTF loader;
	tinygltf::Model model;
	std::string error;
	std::string warning;
	bool ret = loader.LoadASCIIFromFile(&model, &error, &warning, file);

	if (!warning.empty())
		std::cerr << "Warn:" << warning.c_str() << "\n";

	if (!error.empty())
		std::cerr << "Err:" << error.c_str() << "\n";

	if (!ret)
		std::cerr << "Failed to parse glTF\n";

	std::string filename = file.substr(file.find_last_of("/") + 1, file.length());

	if (filename.find("\\") != filename.npos)
		filename = filename.substr(filename.find_last_of("\\") + 1, filename.length());

	//create the resource and Tresource to add onto the map
	std::shared_ptr<TResource<Model>> res = std::make_shared<TResource<Model>>();
	res->SetResource(new Model(&model));
	res->SetName(filename);
	mResources[typeid(Model).name()][filename] = res; //put in the map

}

void ResourceManagerClass::GetTextures(tinygltf::Model* model)
{
	for (size_t i = 0; i < model->textures.size(); i++)
	{
		tinygltf::Texture& tex = model->textures[i];
		if (tex.source > -1) continue;

		tinygltf::Image& img = model->images[tex.source];
		std::string name = img.name.substr(img.name.find_last_of("/") + 1, img.name.length());
		//create Tresource of texture and add to the map
		std::shared_ptr<TResource<Texture>> res = std::make_shared<TResource<Texture>>();
		res->SetResource(new Texture(img));
		res->SetName(name);
		mResources[typeid(Texture).name()][name] = res; //put in the map
	}

}
