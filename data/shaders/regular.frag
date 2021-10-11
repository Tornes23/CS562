#version 430

//output fragmet color
out vec4 FragColor;
//the used textures
layout(location = 0)uniform sampler2D SceneTexture;
layout(location = 1)uniform sampler2D BloomTexture;
uniform bool Bloom;
uniform bool Depth;
uniform float Contrast;
in vec2 UV;

void main()
{
	//setting the output color to the texture sample with ambient color
	if(Bloom)
	{
		const float gamma = 2.2;
        vec3 scene_color = texture(SceneTexture, UV).rgb;      
        vec3 bloom_color = texture(BloomTexture, UV).rgb;
        scene_color += bloom_color; // additive blending

        FragColor = vec4(scene_color, 1.0);
	}
	if(Depth)
	{
	    float color = texture(SceneTexture, UV).r;

		color = (color - (1.0 - Contrast)) / Contrast;
    
		FragColor = vec4(vec3(color), 1); 
	}
	else
		FragColor = texture(SceneTexture, UV);
}