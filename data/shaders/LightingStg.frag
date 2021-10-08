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
layout(location = 0) uniform sampler2D g_posTex;
layout(location = 1) uniform sampler2D g_normalTex;
layout(location = 2) uniform sampler2D g_specularTex;

uniform int LightNum;
uniform Light mLights[MAX_LIGHT_NUM];
uniform vec4 DiffuseColor;
in vec2 UV;

vec3 PointLight(Light light, vec3 initialCol)
{   
    
    //computing the required vectors
    vec3 normal = texture(g_normalTex, UV).rgb; 
    vec3 position = texture(g_posTex, UV).rgb;
    vec3 lightDir = normalize(light.PosInCamSpc - position);

    //computing diffuse value and color
    float diffuseVal = max(dot(normal, lightDir), 0.0);
    vec3 diffuseCol = (diffuseVal * DiffuseColor.xyz) * light.Color.xyz;
    
    //computing the distance
    float dist = length(light.PosInCamSpc - position);

    //adding it to the color
    initialCol += diffuseCol;
    
    //computing specular color
    vec3 viewDir = normalize(-position);//since im cam space cam pos = origin

    //computing the reflection direction
    vec3 reflectDir = reflect(-lightDir, normal);  
    
    //computing the speculat factor and color
    float specVal = texture(g_specularTex, UV).b;
    float shininess = texture(g_specularTex, UV).g;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = vec3(1,1,1) * specVal;  
    
    //computing the attenuation
    float attenuation = min((1.0 / 
                        (light.Attenuation.x + (light.Attenuation.y * dist) 
                        + light.Attenuation.z * (dist * dist))), 1.0);
                        
    //applying the attenuation to the color     
    specular *= attenuation;
    
    //return the color after the lighting
    return specular;
}

vec3 ApplyLighting()
{
    //variable to store the final color
    vec3  finalCol;
    
    vec3 addedLight = vec3(1.0F, 1.0F, 1.0F);

    for(int i = 0; i < LightNum; i++)
        addedLight += PointLight(mLights[i], (mLights[i].Color * DiffuseColor).rgb); 

    //get the texture color
    vec3  textureCol = DiffuseColor.rgb;
    
    //computing the final color
    finalCol += addedLight * textureCol;
    
    //returning the final color
    return finalCol;
}

void main()
{
   //setting the output color to the texture sample
   FragColor = vec4(ApplyLighting(),1);
}