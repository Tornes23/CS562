#version 430

//output fragmet color
out vec4 FragColor;
const float EPSILON = 0.0001;
in vec2 UV;
uniform mat4 invP;
uniform mat4 invV;
uniform mat4 invM;
uniform mat4 Proj;
uniform vec3 ViewVec;
uniform vec2 Size;
uniform float MaxRayBounces;
uniform float MaxRayDist;
uniform float Stride;
uniform float MaxSteps;
uniform float Near;
uniform float Far;
uniform float ZBufferThickness;
layout(location = 0)uniform sampler2D albedoMap;
layout(location = 1)uniform sampler2D normalMap;
layout(location = 3)uniform sampler2D depthMap;

bool TraceRay(vec3 rayDir, out vec3 hitPoint_CS, out vec2 hitPixel);
float Length2(vec2 p0, vec2 p1);
float UnprojectDepth(float depth);
vec3 GetModelPosition(vec2 UV);
vec3 GetWorldPosition(vec2 UV);
vec3 GetViewPosition(vec2 UV);

void main()
{
	vec3 rayDir = -GetViewPosition(UV);
	for(int i = 0; i < MaxRayBounces; i++)
	{
		vec3 hitPoint = vec3(0,0,0);
		vec2 hitPixel = vec2(0,0);
		if(TraceRay(rayDir, hitPoint, hitPixel) == true)
		{
			//based on material compute the reflection/refraction bounces and trace them
			FragColor = texture2D(albedoMap, hitPixel).rgba;
			//rayDir = 
		}
		else
			FragColor = vec4(0,0,0, 1.0);
	}
}

bool TraceRay(vec3 rayDir, out vec3 hitPoint_CS, out vec2 hitPixel)
{
	float rayLength = (ViewVec.z * MaxRayDist) > Near ? (Near / rayDir.z) : MaxRayDist;
	vec3 startpoint_CS = vec3(0, 0, Near);
	vec3 endpoint_CS = rayDir * rayLength + startpoint_CS;

	//projecing points
	vec4 ndc_start = Proj * vec4(startpoint_CS, 1.0);
	vec4 ndc_end = Proj * vec4(endpoint_CS, 1.0);
	//values for perspective division
	float start_div = 1.0 / ndc_start.w;
	float end_div = 1.0 / ndc_end.w;
	//changing the values to be interpolated linearly
	vec3 p_CS = startpoint_CS * start_div; 
	vec3 q_CS = endpoint_CS * end_div; 

	//start and end points in screen space
	vec2 start_SP = startpoint_CS.xy * start_div;
	vec2 end_SP = endpoint_CS.xy * end_div;

	hitPixel = vec2(-1,-1);
	vec2 texelSize = 1.0 / textureSize(albedoMap, 0);
	//checking if the line is at least a pixel of legth, if not make it of length 1 at least to avoid corner case
	end_SP += vec2( Length2(start_SP, end_SP) < EPSILON ? 0.01 : 0.0);
	vec2 delta = end_SP - start_SP;
	//we need to permute the axis on the loop to avoid branching on the GPU, thus increasing performance
	bool permutation = false;
	if(abs(delta.x) < abs(delta.y))
	{
		//the gap on vertical is greater so we swap the axis
		permutation = true;
		delta = delta.yx;
		start_SP = start_SP.yx;
		end_SP = end_SP.yx;
	}

	////////////////////////////////
	//PERFORMING THE DDA ALGORITHM//
	////////////////////////////////

	//all this set up is to mostly work over the X axis
	float stepDir = sign(delta.x);
	float invDX = stepDir / delta.x;
	vec2 dx = vec2(stepDir, delta.y * invDX);
	
	vec3 dx_CS = (endpoint_CS - startpoint_CS) / invDX;
	float dx_Div = (end_div - start_div) / invDX;
	//increasing the derivatives by the setted stride
	dx *= Stride;
	dx_CS *= Stride;
	dx_Div *= Stride;

	//offest values by jitter 
	//start_SP += dx * jitterFraction; 
	//p_CS += dx_CS * jitterFraction;
	//start_div += dx_Div * jitterFraction;

	//now we start to interpolate from p_CS to q_CS (homogeneous start and ending points in camera space)
	//and also from the initial divider start_div to end_div
	vec3 Q = q_CS;
	float div = start_div;

	//we trace on steps of 1/2 a pixel because we treat the pixels as voxels and depth value will be the same either at +1/2 or at -1/2 of the pixel
	float prevZEstimate = startpoint_CS.z;
	float rayMaxZ = prevZEstimate;
	float rayMinZ = prevZEstimate;
	float stepCount = 0.0;
	float sceneMaxDepth = rayMaxZ + 10000;
	//the end point does not get modified any further so we precompute the value to use it for comparison
	float end = end_SP.x * stepDir;

	//we only move on the Z value since we are moving "deeper" onto the scene
	for (vec2 p = start_SP; ((p.x * stepDir) <= end) && 
        (stepCount < MaxSteps) &&
        ((rayMaxZ < sceneMaxDepth - ZBufferThickness) ||
            (rayMinZ > sceneMaxDepth)) &&
        (sceneMaxDepth != 0.0);
        p += dx, Q.z += dx_CS.z, div += dx_Div, stepCount += 1.0)
	{
                
		hitPixel = permutation ? p.yx : p;
		//updating the minimum value
		rayMinZ = prevZEstimate;
		//getting the sample for the next pixel
		rayMaxZ = (dx_CS.z * 0.5 + Q.z) / (dx_Div * 0.5 + div);
		//swap the values to always have the correct one
		if(rayMaxZ < rayMinZ) 
		{
			float temp = rayMaxZ;
			rayMaxZ = rayMinZ;
			rayMinZ = temp;
		}

		//getting the depth
		sceneMaxDepth = texture2D(depthMap, hitPixel).r;
		sceneMaxDepth = UnprojectDepth(sceneMaxDepth);

    }
	//computing the final hit position
	Q.xy += dx_CS.xy * stepCount;
	//the hitpoint on the camera space
	hitPoint_CS = Q * (1.0 / div);

	bool inBounds = (rayMaxZ >= sceneMaxDepth - ZBufferThickness) && (rayMinZ <= sceneMaxDepth);

	return inBounds;
}

float Length2(vec2 p0, vec2 p1)
{
	p1 -= p0;
	return dot(p1, p1);
}
//converts hyperbolic depth to linear
float UnprojectDepth(float depth)
{
	return (Far * Near) / (depth * (Near - Far) + Far);
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
	float depth = texture2D(depthMap, UV).r * 2.0 - 1.0;
    vec2 xy = UV * 2.0 - 1.0;
	vec4 position = vec4(xy, depth, 1.0);
    position = invP * position;
    position /= position.w;
	return position.xyz;
}