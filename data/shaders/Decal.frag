#version 430

//output fragmet color
out vec4 FragColor;
//the used texture
layout(location = 0) uniform sampler2D g_diffuseTex;

uniform vec2 Size;

void main()
{
	vec2 uv = gl_FragCoord.xy / Size;
    vec4 color = texture(g_diffuseTex, uv).rgba;
	FragColor = vec4(1.0);
}