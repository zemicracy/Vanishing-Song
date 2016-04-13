
Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float4 color : COLOR;
};

cbuffer Color : register(b0){
	float4 Color;
}

// entry
float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float4 modelColor;
	float4 finalColor;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

	modelColor = Color;
	// Combine the texture color and the particle color to get the final color result.
    finalColor = textureColor + modelColor;
    return finalColor;
}