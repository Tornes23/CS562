#version 430

layout (location = 0) in vec3 vPosition;
layout (location = 2) in vec2 vTexCoord;

//out variables for the fragment shader
out vec2 UV;

//uniform variables for the transformation
uniform mat4 MVP;
void main()
{
    //applying the transformation to the vertex pos
    gl_Position = MVP * vec4(vPosition, 1.0);
    
    //setting the out variables
    UV = vTexCoord;
}