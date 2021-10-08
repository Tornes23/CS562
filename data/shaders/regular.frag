#version 430

//output fragmet color
out vec4 FragColor;
//the used textures
layout(location = 0)uniform sampler2D diffuseTex;

in vec2 UV;

void main()
{
   //setting the output color to the texture sample
   FragColor = texture(diffuseTex, UV).rgba;
}