#version 430

//output fragmet color
layout(location = 0)out vec4 AmbientOcclusion;
//the used textures
layout(location = 4)uniform sampler2D positionData;

in vec2 UV;
in vec3 Normal;

uniform int mDirectionNum;
uniform int mSteps;
uniform float mBias;
uniform float mRadius;
uniform float mAttenuation;
uniform float mScale;
uniform vec2 Size;
uniform mat4 Proj;

float GetRandomRotation()
{
    return 0.0;
}

vec3 RotateVector(vec3 vec, float angle)
{
    mat3 rot = mat3(cos(angle), -sin(angle), 0,
                    sin(angle), cos(angle), 0,
                    0,          0,          1);

    return rot * vec;
}

vec3 GetSample(vec3 vecInView)
{
    vec4 projected = Proj * vec4(vecInView, 1.0);
    projected /= projected.w;
    return projected.xyz;
}

float HorizonOcclusion()
{
    vec2 texelSize = 1 / Size;
    vec3 initialDir = vec3(1.0, 0, 0);
    float angle = 360.0 / float(mDirectionNum);
    float stepDist = mRadius / float(mSteps);

    //initial sample
    vec3 initialPos= texture(positionData,UV).xyz;
    vec3 initialTan= dFdx(texture(positionData,UV).xyz);
    vec3 initialBitan= dFdy(texture(positionData,UV).xyz);

    float occlusion = 0.0;
    float randomRot = GetRandomRotation();
    for(int i = 0; i < mDirectionNum; i++)
    {
        //getting the rotation angle
        float theta = i * angle + randomRot; 
        //apply random rotation to the direction for better results
        vec3 dir = RotateVector(initialDir, radians(theta));
        //for this direction compute tangent and bitangent
        vec3 tangent = initialTan * cos(radians(theta)) + initialBitan * sin(radians(theta));
        //variables to store the tangent angle and horizon angle
        float tAngle = atan(tangent.z / length(tangent.xy));
        tAngle += mBias;
        float hAngle = 0.0;
        float len = 0.0;

        for(int j = 1; j <= mSteps; j++)
        {
            //get the distance to the next sampling point
            vec3 disp = (float(j) * stepDist) * normalize(dir);
            //get the new sample point UV
            vec3 newPos = initialPos + disp;
            vec2 sampleUV = GetSample(newPos).xy;
            sampleUV = sampleUV * 0.5 + 0.5;
            //sample the position depth
            vec3 samplePos= texture(positionData, sampleUV).xyz; 
            if(samplePos == initialPos)
                continue;
            //float sampleDepth = texture(depthData, sampleUV).r; 
            vec3 horizonVec = samplePos - initialPos;
            //check distance vs radius to discard
            if(length(horizonVec) > mRadius)
                continue;

            //computing the current horizon angle
            float angle = atan(horizonVec.z / length(horizonVec.xy));
            //store if greater than the current maximum store it
            if(angle > hAngle)
            {
                hAngle = angle;
                len = length(horizonVec);
            }

        }

        occlusion += (sin(hAngle) - sin(tAngle)) * (1.0 - pow((len / mRadius), 2)) * mAttenuation;
    }

    occlusion = (occlusion * mScale) / mDirectionNum;

    return 1.0 - occlusion;
}


void main()
{
    float ao = HorizonOcclusion();

    AmbientOcclusion = vec4(vec3(ao), 1.0);
}