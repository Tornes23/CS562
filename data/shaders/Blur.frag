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

vec3 SampleTexture(vec2 offsetUV)
{
    vec2 tex_offset = 1.0 / textureSize(textureData, 0); // gets size of texel
    vec2 uv = vec2(offsetUV.x * tex_offset.x, offsetUV.y * tex_offset.y);
    return texture(textureData, UV + uv).rgb;
}

vec3 BilateralBlur()
{
    vec3 texSample = SampleTexture(vec2(0));
    vec3 result = vec3(0);
    float totalWeight = 0.0;
    float halfKernel = 5.0 / 2.0;

    float fracSpace = -1./(2.0 * 5 * 5);
    float fracRange = -1./(2.0 * RangeSigma * RangeSigma);

    for(float i = -halfKernel; i <= halfKernel; i++)
    {
        for(float j = -halfKernel; j <= halfKernel; j++)
        {
            vec2 newUV = vec2(i, j);
            vec3 newSample = SampleTexture(newUV);
            float distSpace = length(newUV);
            float distRange = length(newSample);
            float spaceWeight = exp(fracSpace * distSpace);
            float rangeWeight = exp(fracRange * distRange);
            float weight = spaceWeight * rangeWeight;
            totalWeight += weight;
            result += newSample * weight;
        }
    }

	//normalizing the value
	return texSample * (result / totalWeight);
}

vec3 GaussianBlur()
{
    vec3 color = texture(textureData, UV).rgb;

    vec2 tex_offset = 1.0 / textureSize(textureData, 0); // gets size of texel
    vec3 result = color * weight[0]; // current contribution using gaussian blur

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