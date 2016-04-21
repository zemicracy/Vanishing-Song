#pragma once
#include "ShaderBase.h"
class HalfFillShader :
	public aetherClass::ShaderBase
{
public:
	struct FillType{
		aetherClass::Vector2 _direction;	//方向	（いらんくなるかも）
		float _interpolation;				//割合
		float _flg;							//１方向　= 0, 角度 = 1;
	public:
		aetherClass::Vector2 _position;		//位置
		float _beginRadius;					//開始角度
		float _endRadius;					//終了角度
	};
public:
	HalfFillShader();
	~HalfFillShader();

	FillType _property;
private:
	void Exception();
	bool InitializeShader()override;
	void FinalizeShader()override;
	bool SetConstantBuffer()override;

	ID3D11Buffer* m_fillTypeBuffer;
};

