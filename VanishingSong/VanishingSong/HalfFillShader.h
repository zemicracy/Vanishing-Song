#pragma once
#include "ShaderBase.h"
class HalfFillShader :
	public aetherClass::ShaderBase
{
public:
	struct FillType{
		FillType(){
			_direction = 0;
			_interpolation = 0;
			_position = 0;
			_beginRadius = 0;
			_endRadius = 0;
			_flg = 0;
		}
		aetherClass::Vector2 _direction;	//方向		//角度の時は１か－１の指定を要する
		float _interpolation;				//割合
		float _flg;							//１方向　= 0, 角度 = 1;
	public:
		/*
		角度以外使用しない
		*/
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

