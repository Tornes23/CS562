#version 400

//output fragmet color
out vec4 FragColor;
out vec4 DiffuseColor;
//the used textures
uniform sampler2D diffuseTex;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

in vec2 UV;
in vec3 Normal;
in vec3 PosInCamSpc;

//vec3 PointLight(vec3 initialCol, vec3 normal)
//{   
//    //computing the distance
//    float distance = length(lightSources[i].PosInCamSpc - fs_in.PosInCamSpc);
//    
//    //computing the light direction
//    vec3 lightDir = normalize(lightSources[i].PosInCamSpc - fs_in.PosInCamSpc);
//    
//    //computing diffuse value and color
//    float diffuseVal = max(dot(normal, lightDir), 0.0);
//    vec3 diffuseCol = diffuseVal * DiffuseColor;
//    
//    //adding it to the color
//    initialCol += diffuseCol;
//    
//    //computing specular color
//    vec3 viewDir = normalize(-fs_in.PosInCamSpc);//since im cam space cam pos = origin
//
//    //computing the reflection direction
//    vec3 reflectDir = reflect(-lightDir, normal);  
//    
//    //computing the speculat factor and color
//    float specVal = texture(specularMap, vec2(fs_in.UV.x, 1 - fs_in.UV.y)).b;
//    float shininess = texture(specularMap, vec2(fs_in.UV.x, 1 - fs_in.UV.y)).g;
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
//    vec3 specular = vec3(1,1,1) * specVal;  
//    
//    //adding it to the color
//    initialCol += specular;
//    
//    //computing the attenuation
//    float attenuation = min((1.0 / 
//                        (lightSources[i].Attenuation.x + (lightSources[i].Attenuation.y * distance) 
//                        + lightSources[i].Attenuation.z * (distance * distance))), 1.0);
//                        
//    //applying the attenuation to the color     
//    initialCol *= attenuation;
//    
//    //return the color after the lighting
//    return initialCol;
//}
//
//vec3 ApplyPhongLight()
//{
//    //variable to store the final color
//    vec3  finalCol;
//    
//    //get the texture color
//    vec3  textureCol = texture(diffuseTex, vec2(fs_in.UV.x, 1 - fs_in.UV.y)).rgb;
//    
//    vec3  normalCol = normalize(texture(normalMap, vec2(fs_in.UV.x, 1- fs_in.UV.y)).rgb * 2.0F - 1.0F);
//    
//    vec3 normal = fs_in.TangentMat * normalCol;
//    
//    //variable to store the color after the lighting process
//    vec3 color = PointLight(DiffuseColor, i, normal);
//    
//    //computing the final color
//    finalCol += color * textureCol;
//    
//    //returning the final color
//    return finalCol;
//}

void main()
{
   //setting the output color to the texture sample
   FragColor = texture(diffuseTex, vec2(UV.x, 1 - UV.y)).rgba;
   //FragColor = vec4(1,1,1,1);
}