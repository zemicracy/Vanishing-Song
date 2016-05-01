
cbuffer Color : register(b0){
	float4 Color;
}

cbuffer FillMode : register(b1){
	float2 _direction;
	float _interpolation;
	float flg;
	
	float2 _position;
	float beginRadius;
	float endRadius;
}

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);


struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 ps_main(PixelInputType input) : SV_TARGET{
	//	float texcoord  = dot(input.tex, _direction);
	if (flg == 0){
		float2 spline = _direction.xy * (_interpolation);
			float texCoord = (input.tex.x*_direction.x) + (input.tex.y*_direction.y);

		float dataLen = length(spline);

		if (-dataLen > texCoord / 2){
			return shaderTexture.Sample(SampleType, input.tex) * Color;
		}
		return float4(0, 0, 0, 0);
	}
	else if (flg == 1){
		float2 convertTex = input.tex - _position;	//íÜêSï‚ê≥
		//float texCoord = length(input.tex) - length((0.5,0.5));
		//float startPos = sin(beginRadius*0.0174532925f);
		//float endPos = sin((beginRadius + (360 * _interpolation))*0.0174532925f)+1;

		float startRad = beginRadius * 0.0174532925f;
		float endRad = ((endRadius * _interpolation) + beginRadius)*0.0174532925f;

		float nowRad = 180 + (atan2(convertTex.y, convertTex.x) * 180 / 3.14);
		nowRad = nowRad *0.0174532925f;
		if (startRad < nowRad && endRad > nowRad){
			return shaderTexture.Sample(SampleType, input.tex) * Color;
		}
	}

	//others
	return float4(0, 0, 0, 0);
}