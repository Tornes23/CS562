#include "GameObjectManager.h"
#include "Utils.h"
#include "ResourceManager.h"

void GameObjectManager::LoadObjects(const nlohmann::json& objs)
{
	for (auto it = objs.begin(); it != objs.end(); it++)
	{
		GameObject go;
		nlohmann::json object = *it;
		//load obj
		object >> go;
		go.mModel = ResourceManager.GetResource<Model>(go.mMesh);
		//load mesh
		mObjects.push_back(go);
	}

}

void GameObjectManager::Update()
{
	for (auto& it : mObjects)
		it.Update();
}
