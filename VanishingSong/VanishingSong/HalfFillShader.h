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
		aetherClass::Vector2 _direction;	//����		//�p�x�̎��͂P���|�P�̎w���v����
		float _interpolation;				//����
		float _flg;							//�P�����@= 0, �p�x = 1;
	public:
		/*
		�p�x�ȊO�g�p���Ȃ�
		*/
		aetherClass::Vector2 _position;		//�ʒu
		float _beginRadius;					//�J�n�p�x
		float _endRadius;					//�I���p�x
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

