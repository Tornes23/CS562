#pragma once
#include <string>
#include "gl/glew.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Shader
{
public:
	Shader(GLenum shaderType, const char* filename);
	const GLuint GetHandle() const;

private:
	std::string mFilename;
	GLenum mType;
	GLuint mHandle;
};

class ShaderProgram
{
public:
	ShaderProgram(std::string vertex, std::string fragment, std::string geometry = "");
	const GLuint GetHandle() const;
	void Create();
	void Free();
	const GLuint GetUniformLoc(const std::string& name) const;

	void SetIntUniform(const std::string& name, int value);
	void SetBoolUniform(const std::string& name, bool value);
	void SetFloatUniform(const std::string& name, float value);
	void SetMatUniform(const std::string& name, float* values);
	void SetVec3Uniform(const std::string& name, glm::vec3 values);
	void SetVec4Uniform(const std::string& name, glm::vec4 values);

	void Use();

private:
	Shader mFragment;
	Shader mVertex;
	Shader mGeometry;

	bool mbGeometry;
	GLuint mHandle;
};

