#version 430

//output fragmet color
out vec4 FragColor;
//the used textures
layout(location = 0)uniform sampler2D SceneTexture;
uniform bool Depth;
uniform float Contrast;
in vec2 UV;

void main()
{
	if(Depth)
	{
	    float color = texture(SceneTexture, UV).r;

		color = (color - (1.0 - Contrast)) / Contrast;
    
		FragColor = vec4(vec3(color), 1); 
	}
	else
		FragColor = texture(SceneTexture, UV);
}