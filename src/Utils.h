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
	void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam);
}

