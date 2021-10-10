#version 430

const int MAX_LIGHT_NUM = 20;

//output fragmet color
out vec4 FragColor;

struct Light
{
    vec3 PosInCamSpc;
    vec4 Color;
    float Radius;
};

//the used textures
layout(location = 0) uniform sampler2D g_diffuseTex;
layout(location = 1) uniform sampler2D g_normalTex;
layout(location = 2) uniform sampler2D g_posTex;
layout(location = 3) uniform sampler2D g_SpecTex;

uniform int LightNum;
uniform Light mLights[MAX_LIGHT_NUM];
uniform vec2 Size;

vec3 PointLight(Light light, vec2 UV)
{   
    
    //computing the required vectors and required data
    vec3 normal = normalize(texture(g_normalTex, UV).rgb); 
    vec3 position = texture(g_posTex, UV).rgb;
    vec3 lightDir = normalize(light.PosInCamSpc - position);
    vec3 viewDir = normalize(-position);//since im cam space cam pos = origin
    vec3 reflectDir = reflect(-lightDir, normal);  
    //getting the shininess and specular values
    float specVal = texture(g_SpecTex, UV).g;
    float shininess = texture(g_SpecTex, UV).b;

    //DIFFUSE COLOR
    float diffuseVal = max(dot(normal, lightDir), 0.0);
    vec3 diffuseCol = diffuseVal * texture(g_diffuseTex, UV).rgb * light.Color.xyz;
    
    //SPECULAR COLOR
    //computing specular color
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    //computing the distance
    float dist = length(light.PosInCamSpc - position);
    //specular color is always white
    vec3 specularCol = vec3(1,1,1) * spec * specVal;  
    
    //computing the attenuation
    float attenuation = 1 - min(dist / light.Radius, 1);
     
     vec3 finalCol = (diffuseCol + specularCol) * attenuation;
    //return the color after the lighting
    return finalCol;
}

vec3 ApplyLighting(vec2 UV)
{
    //variable to store the final color
    //get the texture color and apply ambient lighting
    vec3 finalCol = texture(g_diffuseTex, UV).rgb * 0.2;
    vec3 addedLight = vec3(0, 0, 0);
    vec3 position = texture(g_posTex, UV).xyz;
    for(int i = 0; i < LightNum; i++)
    {
        float dist = length(mLights[i].PosInCamSpc - position);
        if(dist < (mLights[i].Radius * 1.0))
            addedLight += PointLight(mLights[i], UV); 
    }
    
    //computing the final color
    finalCol = addedLight + finalCol;
    
    //returning the final color
    return finalCol;
}

void main()
{
    vec2 uv = gl_FragCoord.xy / Size;
    vec4 color = texture(g_diffuseTex, uv).rgba;
    //setting the output color to the texture sample
    FragColor = vec4(ApplyLighting(uv), color.a);
}