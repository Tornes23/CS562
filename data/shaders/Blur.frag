#version 430

//output fragmet color
layout(location = 0)out vec4 Blur;
//the used texture
layout(location = 0)uniform sampler2D textureData;
//the texture coordinates
in vec2 UV;
//if is horizontal pass
uniform bool HorizontalPass;
uniform bool Gaussian;
uniform vec2 Size;
uniform float RangeSigma;
uniform float mRadius;
//weights for gaussian blur
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

float ComputeWeight(float x, float sigma)
{
    //formula to compute gaussian weights
    return 0.39894 * exp(-0.5 * x * x/(sigma * sigma)) / sigma;
}

vec3 SampleTexture(vec2 offsetUV)
{
    vec2 inverseResolution = 1.0 / Size;
    return texture(textureData, UV + (offsetUV * inverseResolution)).rgb;
}

vec3 BilateralBlur()
{
    vec3 texSample = SampleTexture(vec2(0));
    float centre = texSample.y;
    float weight = 1.0;
    float result = texSample .x * weight;
    float totalWeight = weight;
    float sampleNum = mRadius /2.0;

    for(float i = 1.0; i <= sampleNum; i += 1.0)
    {
        vec2 newUV = vec2(0.0, i);
        if(HorizontalPass)
            newUV = vec2(i, 0.0);

        texSample = SampleTexture(newUV);
        weight = ComputeWeight(texSample.y - centre, RangeSigma);
        result += texSample.x * weight;
        totalWeight += weight;


        texSample = SampleTexture(-newUV);
        weight = ComputeWeight(texSample.y - centre, RangeSigma);
        result += texSample.x * weight;
        totalWeight += weight;

    }

    for(float i = sampleNum; i <= mRadius; i += 2.0)
    {
        vec2 newUV = vec2(0.0, 0.5 + i);
        if(HorizontalPass)
            newUV = vec2(0.5 + i, 0.0);

        texSample = SampleTexture(newUV);
        weight = ComputeWeight(texSample.y - centre, RangeSigma);
        result += texSample.x * weight;
        totalWeight += weight;

        texSample = SampleTexture(-newUV);
        weight = ComputeWeight(texSample.y - centre, RangeSigma);
        result += texSample.x * weight;
        totalWeight += weight;
    }

	//normalizing the value
	return vec3(result / totalWeight);
}

vec3 GaussianBlur()
{
    vec3 color = texture(textureData, UV).rgb;

    vec2 tex_offset = 1.0 / textureSize(textureData, 0); // gets size of texel
    vec3 result = texture(textureData, UV).rgb * weight[0]; // current contribution using gaussian blur

   //different pass for horizontal or vertical
    if(HorizontalPass)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(textureData, UV + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(textureData, UV - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(textureData, UV + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(textureData, UV - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }

    return result;
}

void main()
{
    if(Gaussian)
        Blur = vec4(GaussianBlur(), 1.0);
    else
        Blur = vec4(BilateralBlur(), 1.0);
}