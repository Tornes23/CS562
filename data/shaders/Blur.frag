#version 430

//output fragmet color
layout(location = 1)out vec4 Luminence;
//the used textures
layout(location = 0)uniform sampler2D textureData;
uniform float LumThreshold;
in vec2 UV;

void main()
{
    vec3 color = texture(textureData, UV).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
   //setting the output color if it crosses the threshold
    if(brightness > LumThreshold)
        Luminence = vec4(color, 1.0);
    else
        Luminence = vec4(0.0, 0.0, 0.0, 1.0);
}