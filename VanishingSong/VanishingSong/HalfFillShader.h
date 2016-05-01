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
		
		//角度以外使用しない
		aetherClass::Vector2 _position;		//位置
		float _beginRadius;					//開始角度
		float _endRadius;					//終了角度
	};
	/*
	基本は方向を扱うゲージに対して使う
	*/
	struct DirectionGaugeType{
		DirectionGaugeType(){
			_flg = 0;
			_direction = 0;
			_interpolation = 0;
		}
		HalfFillShader::FillType mCastFillType(){
			HalfFillShader::FillType out;
			out._flg = _flg;
			out._direction = _direction;
			out._interpolation = _interpolation;
			return out;
		}
		public:
			aetherClass::Vector2 _direction;
			float _interpolation;
		private:
			float _flg;
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

