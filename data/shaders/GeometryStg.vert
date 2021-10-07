#version 430

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in vec3 vColor;
layout (location = 4) in vec3 vTangent;

//out variables for the fragment shader
out VS_OUT
{
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 PosInCamSpc;
    vec2 UV;
}vertexCam;

//uniform variables for the transformation
uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 m2w_normal;

void main()
{
    
    //setting the out variables
    vertexCam.UV = vTexCoord;
    vertexCam.PosInCamSpc = vec3(MV * vec4(vPosition, 1.0));
    vertexCam.Normal = normalize(mat3(m2w_normal) * vNormal);
    vertexCam.Tangent = normalize(mat3(MV) * vTangent);
    vec3 bitan = cross(vNormal, vTangent);
    vertexCam.Bitangent = normalize(mat3(MV) * bitan);

    //applying the transformation to the vertex pos
    gl_Position = MVP * vec4(vPosition, 1.0);
}