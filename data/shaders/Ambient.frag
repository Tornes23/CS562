#version 430

//output fragmet color
out vec4 FragColor;
//the used textures
layout(location = 0)uniform sampler2D textureData;
layout(location = 1)uniform sampler2D aoData;

in vec2 UV;
uniform vec4 Ambient;
uniform bool AO;

void main()
{
	//setting the output color to the texture sample with ambient color
	FragColor = Ambient * texture(textureData, UV).rgba;

	if(AO)
		FragColor = FragColor * texture(aoData, UV).rgba;

}