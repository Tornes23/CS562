#include "JSON.h"

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
GameObject& operator>>(const nlohmann::json& j, GameObject& val)
{
    j["mesh"] >> val.mMesh;
    val.mMesh = val.mMesh.substr(val.mMesh.find_last_of("/") + 1, val.mMesh.length());
    j["translate"] >> val.mPos;
    j["rotate"] >> val.mRotation;
    j["scale"] >> val.mScale;
    return val;
}