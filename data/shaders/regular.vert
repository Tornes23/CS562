#version 400 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vUV;
//output fragmet color

uniform mat4 MVP;

void main()
{
	gl_Position =  MVP * vec4(vPosition, 1);
}