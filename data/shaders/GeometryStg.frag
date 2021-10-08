#version 430

//output fragmet color
layout(location = 0)out vec4 DiffuseOut;
layout(location = 1)out vec4 NormalOut;
layout(location = 2)out vec4 PosOut;
//the used textures
layout(location = 0)uniform sampler2D diffuseTex;
layout(location = 1)uniform sampler2D normalMap;
layout(location = 2)uniform sampler2D specularMap;

//in variables for the fragment shader
in VS_OUT
{
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 PosInCamSpc;
    vec2 UV;
    mat3 TanMat;

}vertex;

void main()
{
    //getting the output color to the texture sample
    vec3 diffuse = texture(diffuseTex, vertex.UV).rgb;
    vec3 normal = texture(normalMap, vertex.UV).rgb * 2.0F - 1.0F;
    vec3 specular = texture(specularMap, vertex.UV).rgb;

    DiffuseOut = vec4(diffuse, 0);
    NormalOut = vec4(vertex.TanMat * normal, specular.g);
    PosOut = vec4(vertex.PosInCamSpc, specular.b);
}