#include <iostream>
#include "ResourceManager.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "tinyglft/tiny_gltf.h"


std::string ResourceManagerClass::GetExtension(const std::filesystem::path& path)
{
	std::string name = path.string();
	std::string ext = name.substr(name.find_last_of(".") + 1, name.length());

	return ext;
}

void ResourceManagerClass::LoadModels()
{
	LoadFolder("./data/gltf/");
}

void ResourceManagerClass::LoadFolder(const std::filesystem::path& path)
{
	tinygltf::TinyGLTF loader;
	for (auto& it : std::filesystem::directory_iterator(path))
	{
		std::string ext = GetExtension(it.path());
		if (ext.find("/") != ext.npos)
		{
			LoadFolder(it.path());
		}
		else
		{
			for (unsigned i = 0; i < ext.length(); i++)
				ext[i] = toupper(ext[i]);

			if (ext == "GLTF")
			{
				//load model
				tinygltf::Model model;
				std::string error;
				std::string warning;
				bool ret = loader.LoadASCIIFromFile(&model, &error, &warning, it.path().string());

				if (!warning.empty()) 
					std::cerr << "Warn:" << warning.c_str() << "\n";

				if (!error.empty()) 
					std::cerr<<"Err:" << error.c_str() << "\n";

				if (!ret)
					std::cerr << "Failed to parse glTF\n";
			}

		}
	}
}
