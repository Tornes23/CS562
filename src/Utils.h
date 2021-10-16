#pragma once
#include <string>
#include <gl/glew.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace Utils
{
	void InitSDL();
	void InitGL();
	void ParseShader(std::string& filename, std::string& code);
	void LoadScene(const std::string& filename);
	void LoadResources(const std::string& filename);
	void SaveScreenshot(const std::string& output) ;
	void PerformanceGUI();
	void DebugGUI();

	template <typename T>
	std::string GetTypeName();

	void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam);
}

template <typename T>
std::string Utils::GetTypeName()
{
	std::string strc = "struct ";
	std::string clss = "class ";
	std::string name = typeid(T).name();
	size_t pos = name.find(strc);

	if (pos != name.npos)
		name = name.substr(pos + strc.length(), name.length());

	pos = name.find(clss);
	if (pos != name.npos)
		name = name.substr(pos + clss.length(), name.length());

	return name;
}