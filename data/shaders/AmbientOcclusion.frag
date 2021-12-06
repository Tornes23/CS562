#version 430

//output fragmet color
layout(location = 0)out vec4 AmbientOcclusion;
//the used textures
layout(location = 1)uniform sampler2D normalData;
layout(location = 3)uniform sampler2D depthData;


in vec2 UV;
in vec3 Normal;

uniform int mDirectionNum;
uniform int mSteps;
uniform float mBias;
uniform float mRadius;
uniform float mAttenuation;
uniform float mScale;
uniform mat4 Proj;
uniform mat4 invP;
uniform mat4 invV;
uniform mat4 invM;

vec3 GetModelPosition(vec2 UV);
vec3 GetWorldPosition(vec2 UV);
vec3 GetViewPosition(vec2 UV);

float GetRandomRotation(vec2 uv)
{
    //some fancy formula for the random generation
    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 RotateVector(vec3 vec, float angle)
{
    mat3 rot = mat3(cos(angle), sin(angle), 0,
                    -sin(angle), cos(angle), 0,
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
    vec3 initialDir = vec3(1.0, 0, 0);
    float angle = 360.0 / float(mDirectionNum);
    float stepDist = mRadius / float(mSteps);
    vec3 normal = normalize(texture(normalData, UV).rgb);

    //initial sample
    vec3 initialPos= GetViewPosition(UV);
    vec3 initialTan= normalize(dFdx(initialPos));
    vec3 initialBitan= normalize(dFdy(initialPos));

    float occlusion = 0.0;
    float randomRot = GetRandomRotation(UV) * 360.0;
    //AmbientOcclusion = vec4(normalize(initialBitan),1);
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

        vec2 lastSample = UV;

        for(int j = 1; j <= mSteps; j++)
        {
            //get the distance to the next sampling point
            vec3 disp = (float(j) * stepDist) * normalize(dir);
            //get the new sample point UV
            vec3 newPos = initialPos + disp;
            vec2 sampleUV = GetSample(newPos).xy;
            sampleUV = sampleUV * 0.5 + 0.5;
            //sample the position position
            vec3 samplePos = GetViewPosition(sampleUV); 
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

        float currOcc= (sin(hAngle) - sin(tAngle)) * (1.0 - pow((len / mRadius), 2)) * mAttenuation;
        occlusion += currOcc;
        
    }

    occlusion = (occlusion * mScale) / mDirectionNum;

    return 1.0 - occlusion;
}


void main()
{
    float ao = HorizonOcclusion();

    AmbientOcclusion = vec4(vec3(ao), 1.0);
}


vec3 GetModelPosition(vec2 UV)
{
    //get depth value from texture
	vec4 position = vec4(GetWorldPosition(UV), 1.0);
	position = invM * position;
	return position.xyz;
}

vec3 GetWorldPosition(vec2 UV)
{
	vec4 position = vec4(GetViewPosition(UV), 1.0);
	position = invV * position;
	return position.xyz;
}

vec3 GetViewPosition(vec2 UV)
{
    //get depth value from texture
	float depth = texture2D(depthData, UV).r * 2.0 - 1.0;
    vec2 xy = UV * 2.0 - 1.0;
	vec4 position = vec4(xy, depth, 1.0);
    position = invP * position;
    position /= position.w;
	return position.xyz;
}