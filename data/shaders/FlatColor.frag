#version 430

//output fragmet color
out vec4 FragColor;
uniform vec4 Color;

void main()
{
	FragColor = Color;
}