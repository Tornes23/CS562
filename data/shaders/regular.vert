#version 400 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec3 vTangent;

//out variables for the fragment shader
out vec2 UV;
out vec3 Normal;
out vec3 PosInCamSpc;

//uniform variables for the transformation
uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 m2w_normal;

void main()
{
    //applying the transformation to the vertex pos
    gl_Position = MVP * vec4(vPosition, 1.0);
    
    //setting the out variables
    UV = vTexCoord;
    PosInCamSpc = vec3(MV * vec4(vPosition, 1.0));
    Normal = normalize(vec4(mat3(m2w_normal) * vNormal, 0.0)).xyz;
}