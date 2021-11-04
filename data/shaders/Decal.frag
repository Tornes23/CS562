#version 430

//output fragmet color
layout(location = 0)out vec4 DiffuseOut;
layout(location = 1)out vec4 NormalOut;
layout(location = 2)out vec4 SpecOut;

//the used textures
layout(location = 0)uniform sampler2D diffuseTex;
layout(location = 1)uniform sampler2D normalMap;
layout(location = 2)uniform sampler2D positionMap;
layout(location = 3)uniform sampler2D specularMap;
layout(location = 4)uniform sampler2D depthMap;

//in variables for the fragment shader
in VS_OUT
{
    vec3 Normal;
    vec3 PosInCamSpc;
    vec2 UV;
    mat3 TanMat;

}vertex;


uniform vec2 Size;
uniform mat4 invP;
uniform mat4 invV;
uniform mat4 invM;
uniform int mode;

void main()
{
    ////getting the output color to the texture sample
    //vec3 diffuse = texture(diffuseTex, vertex.UV).rgb;
    //vec3 normal = texture(normalMap, vertex.UV).rgb * 2.0F - 1.0F;
    //vec3 specular = texture(specularMap, vertex.UV).rgb;
	//
    //vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    DiffuseOut = vec4(1.0, 1.0, 1.0, 1.0);
    NormalOut = vec4(1.0, 1.0, 1.0, 1.0);
    SpecOut = vec4(1.0, 1.0, 1.0, 1.0);
	////if rendering the volumes
	//if(mode == 0)
	//{
	//	color = vec4(1.0, 1.0, 1.0, 1.0);
	//}
	//
	//mat4 invMVP = invM * invV * invP;
	//
	//vec2 uv = gl_FragCoord.xy / Size;
	////converting to NDC
	//uv = uv * 2.0 - 1.0;
	//
	////get depth value from texture
	////float depth = texture2D(depthMap, uv).r;
	////depth = depth * 2.0 - 1.0;
	////vec3 pos = vec3(uv, depth);
	//////apply inverse of perspective matrix
	////vec4 final = invMVP * vec4(pos, 1.0);
	//vec4 final = invMVP * texture2D(positionMap, uv).rgba;
	////apply perspective division
	//final /= final.a;
	////if point between -0.5, 0.5 is inside
	//bool inX = final.x >= -0.5 && final.x <= 0.5;
	//bool inY = final.y >= -0.5 && final.y <= 0.5;
	//bool inZ = final.z >= -0.5 && final.z <= 0.5;
	//
	//if(inX && inY && inZ)
	//{
	//	//setting the pixel color as the sample of the diffuse texture (where model space coords are UV)
	//	//offseting the coords to be on 0,1
	//	final += 0.5;
	//	fragColor = texture2D(diffuse, final.xy);
	//}
	//
	////if rendering the projected areas
	//if(mode == 1)
	//{
	//}
	//
	////if rendering the actual decal
	//if(mode == 2)
	//{
	//	color = texture(d_diffuseTex, uv).rgba;
	//}
}