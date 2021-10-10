#include <iostream>
#include "Shader.h"
#include "Utils.h"

ShaderProgram::ShaderProgram(std::string vertex, std::string fragment, std::string geometry) : mVertex(GL_VERTEX_SHADER, vertex.data()),
																							   mFragment(GL_FRAGMENT_SHADER, fragment.data()),
																							   mGeometry(GL_GEOMETRY_SHADER, geometry.data())
{
	mbGeometry = false;

	if (geometry != "")
		mbGeometry = true;

	Create();
}

const GLuint ShaderProgram::GetHandle() const
{
	//returning the handle
	return mHandle;
}

void ShaderProgram::Create()
{
	//creating a shader program
	mHandle = glCreateProgram();

	//attaching the fragment shader
	glAttachShader(mHandle, mFragment.GetHandle());

	//attaching the vertex shader
	glAttachShader(mHandle, mVertex.GetHandle());

	//if it has a geometry shader
	if (mbGeometry)
	{
		//attaching the vertex shader
		glAttachShader(mHandle, mGeometry.GetHandle());
	}

	//linking shaders
	glLinkProgram(mHandle);

	//checking if it linked succesfully
	GLint status;
	glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(mHandle, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	//Detaching the fragment shader
	glDetachShader(mHandle, mFragment.GetHandle());

	//Detaching the vertex shader
	glDetachShader(mHandle, mVertex.GetHandle());

	//Detaching the geometry shader
	if(mbGeometry)
		glDetachShader(mHandle, mGeometry.GetHandle());

}

void ShaderProgram::Free()
{
	glDeleteShader(mVertex.GetHandle());
	glDeleteShader(mFragment.GetHandle());
	
	if(mbGeometry)
		glDeleteShader(mGeometry.GetHandle());
	
	glDeleteProgram(mHandle);
}

const GLuint ShaderProgram::GetUniformLoc(const std::string& name) const
{
	//returning the location of the uniform
	return glGetUniformLocation(mHandle, name.c_str());
}

void ShaderProgram::SetIntUniform(const std::string& name, int value)
{
	//getting the location of the uniform
	GLuint location = GetUniformLoc(name);

	//if it was found
	if (location < 0)
	{
		std::cerr << "Uniform: " << name << " not found" << std::endl;
		return;
	}

	//setting the value
	glUniform1i(location, value);
}

void ShaderProgram::SetBoolUniform(const std::string& name, bool value)
{
	//getting the location of the uniform
	GLuint location = GetUniformLoc(name);

	//if it was found
	if (location < 0)
	{
		std::cerr << "Uniform: " << name << " not found" << std::endl;
		return;
	}

	//setting the value
	glUniform1i(location, value);
}

void ShaderProgram::SetFloatUniform(const std::string& name, float value)
{
	//getting the location of the uniform
	GLuint location = GetUniformLoc(name);

	//if it was found
	if (location < 0)
	{
		std::cerr << "Uniform: " << name << " not found" << std::endl;
		return;
	}

	//setting the value
	glUniform1f(location, value);
}

void ShaderProgram::SetMatUniform(const std::string& name, float* values)
{
	//getting the location of the uniform
	GLuint location = GetUniformLoc(name);

	//if it was found
	if (location < 0)
	{
		std::cerr << "Uniform: " << name << " not found" << std::endl;
		return;
	}

	//setting the value
	glUniformMatrix4fv(location, 1, GL_FALSE, values);

}

void ShaderProgram::SetVec2Uniform(const std::string& name, glm::vec2 values)
{
	//getting the location of the uniform
	GLuint location = GetUniformLoc(name);

	//if it was found
	if (location < 0)
	{
		std::cerr << "Uniform: " << name << " not found" << std::endl;
		return;
	}

	//setting the value
	glUniform2f(location, values.x, values.y);
}

void ShaderProgram::SetVec3Uniform(const std::string& name, glm::vec3  values)
{
	//getting the location of the uniform
	GLuint location = GetUniformLoc(name);

	//if it was found
	if (location < 0)
	{
		std::cerr << "Uniform: " << name << " not found" << std::endl;
		return;
	}

	//setting the value
	glUniform3f(location, values.x, values.y, values.z);

}

void ShaderProgram::SetVec4Uniform(const std::string& name, glm::vec4 values)
{
	//getting the location of the uniform
	GLuint location = GetUniformLoc(name);

	//if it was found
	if (location < 0)
	{
		std::cerr << "Uniform: " << name << " not found" << std::endl;
		return;
	}

	//setting the value
	glUniform4f(location, values.x, values.y, values.z, values.w);

}

void ShaderProgram::SetColorUniform(const std::string& name, Color c)
{
	//getting the location of the uniform
	GLuint location = GetUniformLoc(name);

	//if it was found
	if (location < 0)
	{
		std::cerr << "Uniform: " << name << " not found" << std::endl;
		return;
	}

	//setting the value
	glUniform4fv(location, 1, &c.GetColor()[0]);
}

void ShaderProgram::Use()
{
	// Bind the shader program and this object's VAO
	glUseProgram(mHandle);
}

Shader::Shader(GLenum shaderType, const char* filename)
{
	//setting the member variables
	mType = shaderType;

	mFilename = filename;

	if (mFilename == "")
		return;

	//creating a shader with the given type
	mHandle = glCreateShader(mType);

	//string to store the parsed code
	std::string parsed;

	//calling to parse the code
	Utils::ParseShader(mFilename, parsed);

	//storing it in a const char * because of openGL
	const char* sourceCode = parsed.c_str();

	//setting the source code
	glShaderSource(mHandle, 1, &sourceCode, NULL);

	//compiling the shader code
	glCompileShader(mHandle);

	//checking if it compiled properly
	GLint status;
	glGetShaderiv(mHandle, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;

		glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(mHandle, infoLogLength, NULL, strInfoLog);

		const char* strShaderType = NULL;
		switch (shaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

}

const GLuint Shader::GetHandle() const
{
	//returning the handle
	return mHandle;
}