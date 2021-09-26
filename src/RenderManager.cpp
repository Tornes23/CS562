#include "RenderManager.h"
#include "Utils.h"

void RenderManagerClass::Initialize()
{
	LoadShaders();
}

void RenderManagerClass::LoadLights(const nlohmann::json& lights)
{
	for (auto it = lights.begin(); it != lights.end(); it++)
	{
		Light light;
		nlohmann::json object = *it;
		//load light
		object >> light;
		//load mesh
		mLights.push_back(light);
	}
}

void RenderManagerClass::LoadShaders()
{

}

void RenderManagerClass::Render()
{

}
