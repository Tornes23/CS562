#version 400 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

//out variables for the fragment shader
out vec2 UV;
out vec3 Normal;
//uniform variables for the transformation
uniform mat4 MVP;
uniform mat4 GLTF;

void main()
{
    //setting the out variable
    UV = vTexCoord;
    Normal = vNormal;
    //applying the transformation to the vertex pos
    gl_Position = MVP * GLTF * vec4(vPosition, 1.0);
    
}