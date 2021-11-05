#version 430
layout (location = 0) in vec3 vPosition;

//uniform variables for the transformation
uniform mat4 MVP;
uniform mat4 GLTF;

void main()
{
    //applying the transformation to the vertex pos
    gl_Position = MVP * GLTF * vec4(vPosition, 1.0);
}