
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



float4 ps_main(PixelInputType input) : SV_TARGET
{
	float4 output;

	output = shaderTexture.Sample(SampleType, input.tex);
	return output;
    
}
