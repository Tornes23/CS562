#version 430

//output fragmet color
out vec4 FragColor;
//the used texture
layout(location = 0) uniform sampler2D g_diffuseTex;
layout(location = 1) uniform sampler2D g_normalTex;
layout(location = 2) uniform sampler2D g_specularTex;

layout(location = 3) uniform sampler2D d_diffuseTex;
layout(location = 4) uniform sampler2D d_normalTex;
layout(location = 5) uniform sampler2D d_specularTex;

uniform vec2 Size;

void main()
{
	vec2 uv = gl_FragCoord.xy / Size;
    FragColor = texture(d_diffuseTex, uv).rgba;
}