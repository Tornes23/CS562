#version 430

layout(location = 0) in vec3 vPos;

layout(location = 1)uniform mat4 MVP;

void main()
{   
   //setting the position
   gl_Position = MVP * vec4(vPos, 1.0F);
}