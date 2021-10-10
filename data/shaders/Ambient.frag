#version 430

//output fragmet color
out vec4 FragColor;
//the used textures
layout(location = 0)uniform sampler2D textureData;

in vec2 UV;
uniform vec4 Ambient;

void main()
{
   //setting the output color to the texture sample with ambient color
   FragColor = Ambient * texture(textureData, UV).rgba;
}