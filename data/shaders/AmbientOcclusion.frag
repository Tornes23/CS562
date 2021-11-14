#version 430

//output fragmet color
layout(location = 0)out vec4 AmbientOcclusion;
//the used textures
layout(location = 0)uniform sampler2D textureData;
layout(location = 4)uniform sampler2D Positions;
in vec2 UV;
in vec2 Normal;

uniform int mDirectionNum;
uniform int mSteps;
uniform float mBias;
uniform float mRadius;
uniform float mAttenuation;
uniform float mScale;

void March()
{
    vec3 initialDir = vec3(1.0, 0, 0);
    float angle = 360.0 / mDirectionNum;
    float steps = mRadius / mSteps;
    vec3 tangent = dFdx(texture(Positions,UV).xyz);
    float tAngle;
    float occlusion;

    for(int i = 0; i < mDirectionNum; i++)
    {
        //rotate the initial vector around z to get the direction
        vec3 dir = initialDir;
        //apply random rotation to get better results

        for(int j = 0; j < mSteps; j++)
        {
            //this logic might be wrong, on slides says that we sample and if out of radius ignore
            //if this needs to be done after i recompute the sampling point i need to get the position from texture and check distances
            
            //what im doing is to get uniform samples inside the radius
            //get the distance to the next sampling point
            vec3 disp = (j * steps) * dir;


            //sample the position
            //maube need to vheck distance vs radius to discard
            //check if above or below
            //store if maximum 

            //attenuation??
        }

        float hAngle;

        //check if angle is greater than bias
        //if not greater == not occluded

        occlusion += sin(hAngle) - sin(tAngle);
    }

    occlusion /= mDirectionNum;
}


void main()
{
    March();

    vec3 color = texture(textureData, UV).rgb;
    AmbientOcclusion = vec4(color, 1.0);
}