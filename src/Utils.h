#pragma once
#include <string>
#include "json/json.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "gl/glew.h"
#include "Color.h"
#include "GameObject.h"
#include "Light.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace Utils
{
	void InitSDL();
	void InitGL();
	void ParseShader(std::string& filename, std::string& code);
	void LoadScene(const std::string& filename);
	void LoadResources(const std::string& filename);
	void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam);
}




bool& operator>>(const nlohmann::json& j, bool& val);
short& operator>>(const nlohmann::json& j, short& val);
unsigned short& operator>>(const nlohmann::json& j, unsigned short& val);
int& operator>>(const nlohmann::json& j, int& val);
unsigned int& operator>>(const nlohmann::json& j, unsigned int& val);
float& operator>>(const nlohmann::json& j, float& val);
double& operator>>(const nlohmann::json& j, double& val);
glm::vec2& operator>>(const nlohmann::json& j, glm::vec2& val);
glm::vec3& operator>>(const nlohmann::json& j, glm::vec3& val);
glm::vec4& operator>>(const nlohmann::json& j, glm::vec4& val);
Color& operator>>(const nlohmann::json& j, Color& val);
std::string& operator>>(const nlohmann::json& j, std::string& val);
GameObject& operator>>(const nlohmann::json& j, GameObject& val);
Light& operator>>(const nlohmann::json& j, Light& val);