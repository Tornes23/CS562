#include "GameObjectManager.h"
#include "Utils.h"

void GameObjectManager::LoadObjects(const nlohmann::json& objs)
{
	for (auto it = objs.begin(); it != objs.end(); it++)
	{
		GameObject go;
		nlohmann::json object = *it;
		//load obj
		object >> go;
		//load mesh
		mObjects.push_back(go);
	}

}

void GameObjectManager::Update()
{
	for (auto& it : mObjects)
		it.Update();
}
