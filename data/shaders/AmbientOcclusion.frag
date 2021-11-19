#version 430

//output fragmet color
layout(location = 0)out vec4 AmbientOcclusion;
//the used textures
layout(location = 3)uniform sampler2D depthData;
layout(location = 4)uniform sampler2D positionData;

in vec2 UV;
in vec3 Normal;
in mat4 Projection;

uniform int mDirectionNum;
uniform int mSteps;
uniform float mBias;
uniform float mRadius;
uniform float mAttenuation;
uniform float mScale;
uniform vec2 Size;

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
    vec4 projected = Projection * vec4(vecInView, 1.0);
    projected /= projected.w;
    return projected.xyz;
}

float HorizonOcclusion()
{
    vec2 deltaUV = vec2(1.0/Size.x, 1.0/Size.y);
    vec3 initialDir = vec3(1.0, 0, 0);
    float angle = 360.0 / mDirectionNum;
    float steps = mRadius / mSteps;

    //initial sample
    float initialDepth = texture(depthData, UV).r;
    vec3 initialPos= texture(positionData,UV).xyz;
    vec3 initialTan= dFdx(texture(positionData,UV).xyz);
    vec3 initialBitan= dFdy(texture(positionData,UV).xyz);

    float occlusion = 1.0;
    float randomRot = GetRandomRotation();
    for(int i = 0; i < mDirectionNum; i++)
    {
        //getting the rotation angle
        float theta = i * angle + randomRot; 
        //apply random rotation to the direction for better results
        vec3 dir = RotateVector(initialDir, radians(theta));
        //for this direction compute tangent and bitangent
        vec3 tangent = initialBitan * sin(radians(theta)) + initialTan * cos(radians(theta));
        vec3 bitangent = normalize(cross(tangent, Normal));
        //variables to store the tangent angle and horizon angle
        float tAngle = atan(tangent.z / length(tangent.xy));
        float hAngle = 0.0;

        for(int j = 0; j < mSteps; j++)
        {
            //get the distance to the next sampling point
            vec3 disp = (j * steps) * dir;
            //get the new sample point UV
            vec3 newPos = initialPos + disp;
            vec2 sampleUV = GetSample(newPos).xy;
            //sample the position depth
            vec3 samplePos= texture(positionData, sampleUV).xyz; 
            float sampleDepth = texture(depthData, sampleUV).r; 
            vec3 horizonVec = samplePos - initialPos;

            //check distance vs radius to discard
            if(length(horizonVec) > mRadius)
                continue;

            //computing the current horizon angle
            float angle = atan(horizonVec.z / length(horizonVec.xy));
            //store if greater than the current maximum store it
            if(angle > hAngle)
                hAngle = angle;

            //attenuation??
        }

        //check if angle is greater than bias
        if(hAngle > mBias)
        {
            //if not greater == not occluded
            occlusion += sin(hAngle) - sin(tAngle);
        }
    }

    occlusion /= mDirectionNum;

    return 1.0 - occlusion * mAttenuation;
}


void main()
{
    float ao = HorizonOcclusion();

    AmbientOcclusion = vec4(vec3(ao), 1.0);
}