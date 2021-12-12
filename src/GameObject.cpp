#include <iostream>
#include "ResourceManager.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "glm/geometric.hpp"
#include "glm/gtx/transform.hpp"

GameObject::GameObject(int i)
{
	mbActive = true;
	mPos = glm::vec3(0.0F);
	mRotation = glm::vec3(0.0F);
	mScale = glm::vec3(1.0F);
	mM2W = glm::mat4x4(1.0F);

	mMesh = "Sphere.gltf";
	mModel = ResourceManager.GetResource<Model>("Sphere.gltf");
	mName = mMesh.substr(0, mMesh.find("."));
	mName += std::to_string(i);
}

void GameObject::Update()
{
	//updating the model to world matrix
	mM2W = glm::mat4x4(1.0);
	if(mName != "Sponza.gltf")
		std::cout << "Position In Update = (" << mPos.x << ", " << mPos.y << ", " << mPos.z << ")\n";
	mM2W = glm::translate(mM2W, mPos);
	mM2W = mM2W * glm::rotate(glm::radians(mRotation.x), glm::vec3(1, 0, 0));
	mM2W = mM2W * glm::rotate(glm::radians(mRotation.y), glm::vec3(0, 1, 0));
	mM2W = mM2W * glm::rotate(glm::radians(mRotation.z), glm::vec3(0, 0, 1));
	mM2W = glm::scale(mScale);
}

void GameObject::Edit()
{
	//pushing the id
	ImGui::PushID(mName.c_str());

	if (ImGui::CollapsingHeader(mName.c_str()))
	{
		if (ImGui::TreeNode("Transform"))
		{
			ImGui::DragFloat3("Position", &mPos[0]);
			std::cout << "Position In Edit = (" << mPos.x << ", " << mPos.y << ", " << mPos.z << ")\n";
			ImGui::DragFloat3("Scale", &mScale[0]);

			ImGui::TreePop();
		}

		//to edit the model
		//if (ImGui::TreeNode("Model"))
		//{
		//	int index = ResourceManager.GetResourceIndex<Model>(mModel->GetName());
		//	auto model_options = ResourceManager.GetResources<Model>();
		//	int count = static_cast<int>(model_options.size());
		//	//combo of the possible models
		//	if (ImGui::Combo("Object Model", &index, &model_options[0], count, count + 1))
		//	{
		//		mModel = ResourceManager.GetResource<Model>(model_options[index]);
		//
		//		auto& mat = mModel->GetMaterialVector();
		//		for (auto m : mat)
		//		{
		//			ImGui::PushID(mName.data());
		//			//to edit the model
		//			if (ImGui::TreeNode("Model"))
		//			{
		//				int index = ResourceManager.GetResourceIndex<Material>(m->GetName());
		//				auto material_options = ResourceManager.GetResources<Material>();
		//				int count = static_cast<int>(material_options.size());
		//				//combo of the possible models
		//				if (ImGui::Combo("Object Material", &index, &material_options[0], count, count + 1))
		//					m = ResourceManager.GetResource<Material>(material_options[index]);
		//
		//				ImGui::TreePop();
		//			}
		//			//popping the id
		//			ImGui::PopID();
		//		}
		//	}
		//
		//	ImGui::TreePop();
		//}
	}

	//popping the id
	ImGui::PopID();
}
