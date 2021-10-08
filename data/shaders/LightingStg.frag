#version 430

const int MAX_LIGHT_NUM = 20;

//output fragmet color
out vec4 FragColor;

struct Light
{
    vec3 PosInCamSpc;
    vec4 Color;
    vec3 Attenuation;
};

//the used textures
layout(location = 0) uniform sampler2D g_diffuseTex;
layout(location = 1) uniform sampler2D g_normalTex;
layout(location = 2) uniform sampler2D g_posTex;

uniform int LightNum;
uniform Light mLights[MAX_LIGHT_NUM];
uniform vec4 DiffuseColor;
in vec2 UV;

vec3 PointLight(Light light)
{   
    
    //computing the required vectors and required data
    vec3 normal = normalize(texture(g_normalTex, UV).rgb); 
    vec3 position = texture(g_posTex, UV).rgb;
    vec3 lightDir = normalize(light.PosInCamSpc - position);
    vec3 viewDir = normalize(-position);//since im cam space cam pos = origin
    vec3 reflectDir = reflect(-lightDir, normal);  

    //getting the shininess and specular values
    float specVal = texture(g_posTex, UV).a;
    float shininess = texture(g_normalTex, UV).a;

    //AMBIENT COLOR
    vec3 ambientCol = (light.Color * DiffuseColor).xyz;

    //DIFFUSE COLOR
    float diffuseVal = max(dot(normal, lightDir), 0.0);
    vec3 diffuseCol = (diffuseVal * texture(g_diffuseTex, UV).rgb) * light.Color.xyz;
    
    //SPECULAR COLOR
    //computing specular color
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    //computing the distance
    float dist = length(light.PosInCamSpc - position);
    //specular color is always white
    vec3 specularCol = vec3(1,1,1) * specVal;  
    
    //computing the attenuation
    float attenuation = min((1.0 / 
                        (light.Attenuation.x + (light.Attenuation.y * dist) 
                        + light.Attenuation.z * (dist * dist))), 1.0);
     
     vec3 finalCol = (ambientCol + (diffuseCol + specularCol)) * attenuation;
    //return the color after the lighting
    return finalCol;
}

vec3 ApplyLighting()
{
    //variable to store the final color
    vec3  finalCol;
    
    vec3 addedLight = vec3(0.0F, 0.0F, 0.0F);

    for(int i = 0; i < LightNum; i++)
        addedLight += PointLight(mLights[i]); 

    //get the texture color
    vec3  textureCol = texture(g_diffuseTex, UV).rgb;
    
    //computing the final color
    finalCol += addedLight * textureCol;
    
    //returning the final color
    return finalCol;
}

void main()
{
    vec4 color = texture(g_diffuseTex, UV).rgba;
    //setting the output color to the texture sample
    FragColor = vec4(ApplyLighting(), color.a);
}