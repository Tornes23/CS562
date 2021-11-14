#version 430

//output fragmet color
out vec4 FragColor;
in vec4 PosInCamSpc;
struct Light
{
    vec3 PosInCamSpc;
    vec4 Color;
    float Radius;
};

//the used textures
layout(location = 0) uniform sampler2D g_diffuseTex;
layout(location = 1) uniform sampler2D g_normalTex;
layout(location = 2) uniform sampler2D g_SpecTex;
layout(location = 3) uniform sampler2D g_DepthTex;
layout(location = 4) uniform sampler2D g_PosTex;

uniform Light mLight;
uniform vec2 Size;
uniform mat4 invP;

vec3 PointLight(Light light, vec2 UV)
{   
    //computing the required vectors and required data
    vec3 normal = normalize(texture(g_normalTex, UV).rgb); 

    //get depth value from texture
	float depth = texture2D(g_DepthTex, UV).r * 2.0 - 1.0;
    vec2 xy = UV * 2.0 - 1.0;
	vec4 position = vec4(xy, depth, 1.0);
    position = invP * position;
    position /= position.w;

    vec3 lightDir = normalize(light.PosInCamSpc - position.xyz);
    vec3 viewDir = normalize(-position.xyz);//since im cam space cam pos = origin
    vec3 reflectDir = reflect(-lightDir, normal);  
    //getting the shininess and specular values
    float shininess = texture(g_SpecTex, UV).g;
    float specVal = texture(g_SpecTex, UV).b;

    //DIFFUSE COLOR
    float diffuseVal = max(dot(normal, lightDir), 0.0);
    vec4 diffuseCol = diffuseVal * texture(g_diffuseTex, UV).rgba * light.Color;
    
    //SPECULAR COLOR
    //computing specular color
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    //computing the distance
    float dist = length(light.PosInCamSpc - position.xyz);
    //specular color is always white
    vec3 specularCol = vec3(1,1,1) * spec * specVal;  
    
    //computing the attenuation
    float attenuation = 1 - min( dist / mLight.Radius, 1.0);
     
    vec3 finalCol = (diffuseCol.xyz + specularCol) * attenuation;
    //return the color after the lighting
    return finalCol;
}

void main()
{
    vec2 uv = gl_FragCoord.xy / Size;
    vec4 color = texture(g_diffuseTex, uv).rgba;
    //setting the output color to the texture sample
    FragColor = vec4(PointLight(mLight, uv), color.a);
}