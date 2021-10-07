#version 430

//output fragmet color
layout(location = 0)out vec4 DiffuseOut;
layout(location = 1)out vec4 NormalOut;
layout(location = 2)out vec4 PosOut;
//the used textures
uniform sampler2D diffuseTex;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

//in variables for the fragment shader
in VS_OUT
{
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 PosInCamSpc;
    vec2 UV;
}vertex;

void main()
{
    //getting the output color to the texture sample
    mat3 tan2cam = mat3(normalize(vertex.Tangent), normalize(vertex.Bitangent), normalize(vertex.Normal));
    vec3 diffuse = texture(diffuseTex, vertex.UV).rgb;
    vec3 normal = 2.0 * texture(normalMap, vertex.UV).rgb - 1;
    vec3 specular = texture(specularMap, vertex.UV).rgb;

    DiffuseOut = vec4(diffuse, specular.r);
    NormalOut = vec4(tan2cam * normal, 0);
    PosOut = vec4(vertex.PosInCamSpc, 0);
}