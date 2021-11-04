#version 430

//output fragmet color
layout(location = 0)out vec4 DiffuseOut;
layout(location = 1)out vec4 NormalOut;
layout(location = 2)out vec4 SpecOut;

//the used textures
layout(location = 0)uniform sampler2D diffuseTex;
layout(location = 1)uniform sampler2D normalMap;
layout(location = 2)uniform sampler2D specularMap;
layout(location = 3)uniform sampler2D depthMap;

uniform vec2 Size;
uniform mat4 invP;
uniform mat4 invV;
uniform mat4 invM;
uniform int Mode;

vec3 GetModelPosition(vec2 UV);
void ShowDecalTexture();
void ShowDecalVolume();
void ShowDecalProjected();

void main()
{
	if(Mode == 0)
		ShowDecalVolume();
	if(Mode == 1)
		ShowDecalProjected();
	if(Mode == 2)
		ShowDecalTexture();

}

vec3 GetModelPosition(vec2 UV)
{
    //get depth value from texture
	float depth = texture2D(depthMap, UV).r * 2.0 - 1.0;
    vec2 xy = UV * 2.0 - 1.0;
	vec4 position = vec4(xy, depth, 1.0);
    position = invP * position;
    position /= position.w;

	position = invV * position;
	position = invM * position;

	return position.xyz;
}

void ShowDecalTexture()
{
	vec2 uv = gl_FragCoord.xy / Size;
	vec3 pos = GetModelPosition(uv);
	//if point between -0.5, 0.5 is inside of volume
	bool inX = pos.x >= -0.5 && pos.x <= 0.5;
	bool inY = pos.y >= -0.5 && pos.y <= 0.5;
	bool inZ = pos.z >= -0.5 && pos.z <= 0.5;
	

	if(inX && inY && inZ)
	{
		//setting the pixel color as the sample of the diffuse texture (where model space coords are UV)
		//offseting the coords to be on 0,1
		vec2 newUV = pos.xy + 0.5;

		vec3 specular = texture(specularMap, newUV).rgb;

		vec3 normal = normalize(texture(normalMap, newUV).rgb * 2.0 - 1.0);
		vec3 tangent = normalize(dFdy(pos));
		vec3 bitan = normalize(dFdy(pos));
		mat3 tbn = mat3(tangent, bitan, normal);

		DiffuseOut = texture2D(diffuseTex, newUV);
		NormalOut = vec4(tbn * normal, 1.0F);
		SpecOut = vec4(0, specular.g, specular.b, 1.0F);

	}
}

void ShowDecalVolume()
{
	DiffuseOut = vec4(1.0);
}
void ShowDecalProjected()
{
	vec2 uv = gl_FragCoord.xy / Size;
	vec3 pos = GetModelPosition(uv);
	//if point between -0.5, 0.5 is inside of volume
	bool inX = pos.x >= -0.5 && pos.x <= 0.5;
	bool inY = pos.y >= -0.5 && pos.y <= 0.5;
	bool inZ = pos.z >= -0.5 && pos.z <= 0.5;
	
	if(inX && inY && inZ)
		DiffuseOut = vec4(1.0);
}