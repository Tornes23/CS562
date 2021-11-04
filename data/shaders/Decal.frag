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
uniform float ClipAngle;

vec3 GetModelPosition(vec2 UV);
vec3 GetWorldPosition(vec2 UV);
vec3 GetViewPosition(vec2 UV);
vec2 ShowDecalTexture(vec2 UV);
vec2 ShowDecalProjected(vec2 UV);
void ShowDecalVolume();

void main()
{
	vec2 uv = gl_FragCoord.xy / Size;
	vec2 newUV = vec2(0,0);

	if(Mode == 0)
		ShowDecalVolume();
	if(Mode == 1)
		newUV = ShowDecalProjected(uv);
	if(Mode == 2)
		newUV = ShowDecalTexture(uv);
		
	vec3 posInCam = GetViewPosition(uv);
	vec3 specular = texture(specularMap, newUV).rgb;
	vec3 normal = normalize(texture(normalMap, newUV).rgb * 2.0 - 1.0);
	vec3 tangent = normalize(dFdx(posInCam));
	vec3 bitan = normalize(dFdy(posInCam));
	mat3 tbn = mat3(tangent, bitan, normal);

	NormalOut = vec4(tbn * normal, 1.0);
	SpecOut = vec4(0, specular.g, specular.b, 1.0);

}

vec3 GetModelPosition(vec2 UV)
{
    //get depth value from texture
	vec4 position = vec4(GetWorldPosition(UV), 1.0);
	position = invM * position;
	return position.xyz;
}

vec3 GetWorldPosition(vec2 UV)
{
	vec4 position = vec4(GetViewPosition(UV), 1.0);
	position = invV * position;
	return position.xyz;
}

vec3 GetViewPosition(vec2 UV)
{
    //get depth value from texture
	float depth = texture2D(depthMap, UV).r * 2.0 - 1.0;
    vec2 xy = UV * 2.0 - 1.0;
	vec4 position = vec4(xy, depth, 1.0);
    position = invP * position;
    position /= position.w;
	return position.xyz;
}

vec2 ShowDecalTexture(vec2 UV)
{
	vec3 pos = GetModelPosition(UV);
	//if point between -0.5, 0.5 is inside of volume
	bool outX = pos.x < -1.0 || pos.x > 1.0;
	bool outY = pos.y < -1.0 || pos.y > 1.0;
	bool outZ = pos.z < -1.0 || pos.z > 1.0;

	//if is outside the volume
	if(outX || outY || outZ)
		discard;
	
	//setting the pixel color as the sample of the diffuse texture (where model space coords are UV)
	//offseting the coords to be on 0,1
	vec2 newUV = pos.xy + 0.5;
	float alpha = texture(diffuseTex, newUV).a;
	//compute the angle between normal and surface if smaler than set value discard
	//float angle = cross();
	float angle = 1.0;
	if(alpha == 0.0 || angle < ClipAngle)
		discard;

	DiffuseOut = texture2D(diffuseTex, newUV).rgba;

	return newUV;
}

void ShowDecalVolume()
{
	DiffuseOut = vec4(1.0);
}

vec2 ShowDecalProjected(vec2 UV)
{
	vec3 pos = GetModelPosition(UV);
	//if point between -0.5, 0.5 is inside of volume
	bool outX = pos.x < -1.0 || pos.x > 1.0;
	bool outY = pos.y < -1.0 || pos.y > 1.0;
	bool outZ = pos.z < -1.0 || pos.z > 1.0;
	
	//if is inside the volume
	if(outX || outY || outZ)
		discard;

	DiffuseOut = vec4(pos, 1.0);

	return pos.xy + 0.5;
}