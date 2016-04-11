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

float4 ps_main(PixelInputType input) : SV_TARGET
{

	input.tex *= 0.7;
	input.tex += 0.1;

	float4 final = shaderTexture.Sample(SampleType, input.tex);

	return final;
}