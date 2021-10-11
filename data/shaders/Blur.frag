#version 430

//output fragmet color
layout(location = 0)out vec4 Blur;
//the used texture
layout(location = 0)uniform sampler2D textureData;
//the texture coordinates
in vec2 UV;
//if is horizontal pass
uniform bool HorizontalPass;
//wights for gaussian blur
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec3 color = texture(textureData, UV).rgb;

    vec2 tex_offset = 1.0 / textureSize(textureData, 0); // gets size of texel
    vec3 result = texture(textureData, UV).rgb * weight[0]; // current contribution using gaussian blur

   //different pass for horizontal or vertical
    if(HorizontalPass)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(textureData, UV + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(textureData, UV - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(textureData, UV + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(textureData, UV - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }

    Blur = vec4(result, 1.0);
}