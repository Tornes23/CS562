#pragma once
#include <vector>
#include "json/json.hpp"
#include "GameObject.h"

class GameObjectManager
{
public:
	//singleton stuff
	GameObjectManager(GameObjectManager const&) = delete;
	void operator=(GameObjectManager const&) = delete;
	static GameObjectManager& GetInstance()
	{
		static GameObjectManager instance;
		return instance;
	}
	//member functions
	void LoadObjects(const nlohmann::json& j);
	void Update();

private:
	GameObjectManager() { }
	std::vector<GameObject> mObjects;
};

#define GOManager (GameObjectManager::GetInstance())