
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
	float4 Color;
}


float4 ps_main(PixelInputType input) : SV_TARGET
{
	float4 output;
	output = shaderTexture.Sample(SampleType, input.tex);
	if(Color.w > 0){
	float4 col2 = Color;
	col2 = (col2 - (1-Color.a));
	output.rgb = col2.rgb + output.rgb;
	}

	return output;
    
}
