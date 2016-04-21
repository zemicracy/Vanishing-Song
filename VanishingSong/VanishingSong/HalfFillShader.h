#pragma once
#include "ShaderBase.h"
class HalfFillShader :
	public aetherClass::ShaderBase
{
public:
	struct FillType{
		aetherClass::Vector2 _direction;	//����	�i����񂭂Ȃ邩���j
		float _interpolation;				//����
		float _flg;							//�P�����@= 0, �p�x = 1;
	public:
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

