#version 430

//output fragmet color
out vec4 FragColor;
//the used textures
layout(location = 0)uniform sampler2D SceneTexture;
layout(location = 1)uniform sampler2D BlurredTexture;
in vec2 UV;

void main()
{
    vec3 scene_color = texture(SceneTexture, UV).rgb;      
    vec3 bloom_color = texture(BlurredTexture, UV).rgb;
    scene_color += bloom_color; // additive blending

    FragColor = vec4(scene_color, 1.0);
}