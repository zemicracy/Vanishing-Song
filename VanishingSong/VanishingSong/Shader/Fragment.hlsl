
// Global
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);
	
// Typedef
struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

cbuffer Color : register(b0){
	float4 color;
}

/*
	０	基本色のみ
	１	テクスチャのみ
	２	色＋テクスチャ
*/
static const float kOnlyColor = 0;
static const float kOnlyTexture = 1;
static const float kMixColor = 2;


cbuffer Mode : register(b1){
	float mode;
	float3 Padding;
}



float4 ps_main(PixelInputType input) : SV_TARGET
{
	if (mode == kOnlyColor){
		return color;
	}
	else if (mode == kOnlyTexture){
		float4 output;
		output = shaderTexture.Sample(SampleType, input.tex);
		return output;
	}
	else if (mode == kMixColor){
		float4 output;
		output = shaderTexture.Sample(SampleType, input.tex) * color;
		return output;
	}
	return float4(0, 0, 0, 0);
}
