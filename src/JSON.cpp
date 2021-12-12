#include "JSON.h"
#include "ResourceManager.h"

bool& operator>>(const nlohmann::json& j, bool& val) { val = j; return val; }
short& operator>>(const nlohmann::json& j, short& val) { val = j; return val; }
unsigned short& operator>>(const nlohmann::json& j, unsigned short& val) { val = j; return val; }
int& operator>>(const nlohmann::json& j, int& val) { val = j; return val; }
unsigned int& operator>>(const nlohmann::json& j, unsigned int& val) { val = j; return val; }
float& operator>>(const nlohmann::json& j, float& val) { val = j; return val; }
double& operator>>(const nlohmann::json& j, double& val) { val = j; return val; }
glm::vec2& operator>>(const nlohmann::json& j, glm::vec2& val) { j["x"] >> val.x; j["y"] >> val.y; return val; }
glm::vec3& operator>>(const nlohmann::json& j, glm::vec3& val) { j["x"] >> val.x; j["y"] >> val.y; j["z"] >> val.z; return val; }
glm::vec4& operator>>(const nlohmann::json& j, glm::vec4& val) { j["x"] >> val.x; j["y"] >> val.y; j["z"] >> val.z; j["w"] >> val.w; return val; }
Color& operator>>(const nlohmann::json& j, Color& val) { val = { j["x"], j["y"], j["z"] }; return val; }
std::string& operator>>(const nlohmann::json& j, std::string& val) { val = j.get<std::string>(); return val; }
Light& operator>>(const nlohmann::json& j, Light& val) { j["position"] >> val.mPos; j["color"] >> val.mColor; j["radius"] >> val.mRadius; return val; }
Decal& operator>>(const nlohmann::json& j, Decal& val)
{
    std::string name;
    j["diffuse"] >> name;
    name = name.substr(name.find_last_of("/") + 1, name.length());
    //Get resource
    val.mDiffuse = ResourceManager.GetResource<Texture>(name);

    j["normal"] >> name;
    name = name.substr(name.find_last_of("/") + 1, name.length());
    //Get resource
    val.mNormal = ResourceManager.GetResource<Texture>(name);
    j["metallic"] >> name;
    name = name.substr(name.find_last_of("/") + 1, name.length());
    //Get resource
    val.mMetallic = ResourceManager.GetResource<Texture>(name);

    j["translate"] >> val.mPosition;
    j["rotate"] >> val.mRotation;
    j["scale"] >> val.mScale;

    val.mModel = ResourceManager.GetResource<Model>("Cube.gltf");

    return val;
}
GameObject& operator>>(const nlohmann::json& j, GameObject& val)
{
    j["mesh"] >> val.mMesh;
    j["translate"] >> val.mPos;
    j["rotate"] >> val.mRotation;
    j["scale"] >> val.mScale;
    return val;
}

void operator<<(nlohmann::json& j, bool& val) { j = val; }
void operator<<( nlohmann::json& j, short& val) { j = val; }
void operator<<( nlohmann::json& j, unsigned short& val) { j = val; }
void operator<<( nlohmann::json& j, int& val) { j = val; }
void operator<<( nlohmann::json& j, unsigned int& val) { j = val; }
void operator<<( nlohmann::json& j, float& val) { j = val; }
void operator<<( nlohmann::json& j, double& val) { j = val; }
void operator<<( nlohmann::json& j, glm::vec2& val) { j["x"] << val.x; j["y"] << val.y; }
void operator<<( nlohmann::json& j, glm::vec3& val) { j["x"] << val.x; j["y"] << val.y; j["z"] << val.z;}
void operator<<( nlohmann::json& j, glm::vec4& val) { j["x"] << val.x; j["y"] << val.y; j["z"] << val.z; j["w"] << val.w; }
void operator<<(nlohmann::json& j, Color& val) { j["r"] << val.mR; j["g"] << val.mG; j["b"] << val.mB; j["a"] << val.mA;  }
void operator<<( nlohmann::json& j, std::string& val) { j = val.c_str();  }
void operator<<( nlohmann::json& j, Light& val) { j["position"] << val.mPos; j["color"] << val.mColor; j["radius"] << val.mRadius; }
void operator<<( nlohmann::json& j, Decal& val)
{
    //std::string name;
    //j["diffuse"] << name;
    //name << name.substr(name.find_last_of("/") + 1, name.length());
    ////Get resource
    //val.mDiffuse << ResourceManager.GetResource<Texture>(name);
    //
    //j["normal"] << name;
    //name << name.substr(name.find_last_of("/") + 1, name.length());
    ////Get resource
    //val.mNormal << ResourceManager.GetResource<Texture>(name);
    //j["metallic"] << name;
    //name << name.substr(name.find_last_of("/") + 1, name.length());
    ////Get resource
    //val.mMetallic << ResourceManager.GetResource<Texture>(name);
    //
    //j["translate"] << val.mPosition;
    //j["rotate"] << val.mRotation;
    //j["scale"] << val.mScale;
    //
}
void operator<<( nlohmann::json& j, GameObject& val)
{
    std::string prefix = "data/gltf/";
    j["mesh"] << val.mMesh;
    j["translate"] << val.mPos;
    j["rotate"] << val.mRotation;
    j["scale"] << val.mScale;
}