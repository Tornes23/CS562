#version 430

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in vec3 vTangent;

//out variables for the fragment shader
out VS_OUT
{
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec2 UV;
    mat3 TanMat;

}vertexCam;

//uniform variables for the transformation
uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 m2v_normal;
uniform mat4 GLTF;

void main()
{
    
    //setting the out variables
    vertexCam.UV = vTexCoord;
    vertexCam.Normal = normalize(mat3(m2v_normal) * vNormal);
    vertexCam.Tangent = normalize(vec3(MV * GLTF * vec4(vTangent, 0.0F)));

    vec3 bitan = normalize(cross(vNormal, vTangent));
    vertexCam.TanMat = mat3(normalize(vTangent), normalize(bitan), normalize(vNormal));
    vertexCam.Bitangent = normalize(mat3(MV * GLTF) * bitan);

    //applying the transformation to the vertex pos
    gl_Position = MVP * GLTF * vec4(vPosition, 1.0);
}