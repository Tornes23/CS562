#include <string>
#include "GameObjectManager.h"
#include "imgui/imgui.h"
#include "JSON.h"
#include "ResourceManager.h"

void GameObjectManager::LoadObjects(const nlohmann::json& objs)
{
	mObjects.clear();
	int i = 0;
	for (auto it = objs.begin(); it != objs.end(); it++)
	{
		GameObject go;
		nlohmann::json object = *it;
		//load obj
		object >> go;
		std::string modelName = go.mMesh.substr(go.mMesh.find_last_of("/") + 1, go.mMesh.length());
		go.mModel = ResourceManager.GetResource<Model>(modelName);
		go.mName = modelName.substr(0, modelName.find("."));
		go.mName += std::to_string(i);
		//load mesh
		mObjects.push_back(go);
		i++;
	}

}

void GameObjectManager::AddObject(int i)
{
	mObjects.push_back({i});
}

void GameObjectManager::Update()
{
	//creating the window
	if (!ImGui::Begin("Game Objects"))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	if (ImGui::Button("New Game Object"))
	{
		AddObject(static_cast<int>(mObjects.size()));
	}

	for (auto& it : mObjects)
	{
		it.Update();

		if (it.mName == "Sponza0")
			continue;
		it.Edit();
	}

	ImGui::End();
}

void GameObjectManager::Save(nlohmann::json& j)
{
	nlohmann::json& objs = j["objects"];
	for (auto& obj : mObjects)
	{
		nlohmann::json j_temp;
		j_temp << obj;
		objs.push_back(j_temp);
	}
}

