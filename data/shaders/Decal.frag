#version 430

//output fragmet color
out vec4 FragColor;
//the used texture
layout(location = 0) uniform sampler2D g_depthTex;
layout(location = 1) uniform sampler2D g_positionTex;
layout(location = 2) uniform sampler2D g_specularTex;

layout(location = 3) uniform sampler2D d_diffuseTex;
layout(location = 4) uniform sampler2D d_normalTex;
layout(location = 5) uniform sampler2D d_specularTex;

uniform vec2 Size;
uniform mat4 invMVP;
uniform int mode;

void main()
{
	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
	//if rendering the volumes
	if(mode == 0)
	{
		color = vec4(1.0, 1.0, 1.0, 1.0);
	}

	vec2 uv = gl_FragCoord.xy / Size;
	//converting to NDC
	uv = uv * 2.0 - 1.0;
	//get depth value from texture
	//
	//apply inverse of perspective matrix
	//apply perspective division
	//pass to world space
	//apply inverse of M2W to get the point on cubes models space

	//if point between -0.5, 0.5 is inside

	//if rendering the projected areas
	if(mode == 1)
	{
	}

	//if rendering the actual decal
	if(mode == 2)
	{
		color = texture(d_diffuseTex, uv).rgba;
	}
	
	FragColor = color;
}