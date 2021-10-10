#version 400 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in vec3 vColor;
layout (location = 4) in vec3 vTangent;

//out variables for the fragment shader
out vec2 UV;

//uniform variables for the transformation
uniform mat4 MVP;

void main()
{
    //setting the out variable
    UV = vTexCoord;

    //applying the transformation to the vertex pos
    gl_Position = MVP * vec4(vPosition, 1.0) * 0.2;
    
}