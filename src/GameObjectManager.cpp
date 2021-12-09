#include "GameObjectManager.h"
#include "imgui/imgui.h"
#include "JSON.h"
#include "ResourceManager.h"

void GameObjectManager::LoadObjects(const nlohmann::json& objs)
{
	mObjects.clear();
	for (auto it = objs.begin(); it != objs.end(); it++)
	{
		GameObject go;
		nlohmann::json object = *it;
		//load obj
		object >> go;
		go.mModel = ResourceManager.GetResource<Model>(go.mMesh);
		go.mName = go.mMesh;
		//load mesh
		mObjects.push_back(go);
	}

}

void GameObjectManager::AddObject()
{
	mObjects.push_back({});
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

	for (auto& it : mObjects)
	{
		it.Update();

		if (it.mName == "Sponza.gltf")
			continue;
		it.Edit();
	}

	ImGui::End();
}

