
// Global
Texture2D basicTexture: register(t0);
Texture2D addTexture: register(t1);

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
	output = basicTexture.Sample(SampleType, input.tex);
	if(Color.w > 0){
	float4 col2 = Color;
	col2 = (col2 - (1-Color.a));
	output.a = col2.a + output.a;
	output.rgb = saturate(col2).rgb + output.rgb;
	}
	float4 add;
	add = addTexture.Sample(SampleType,input.tex);

	float Sf = 1;
	float Df = 1;

	float alpha = add.a + output.a;
	output.rgb = (add.rgb*add.a*Sf+output.rgb*output.a*Df)/alpha;

	return output;
    
}
