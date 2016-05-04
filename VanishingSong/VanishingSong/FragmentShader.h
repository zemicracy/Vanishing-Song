#pragma once
#include "ShaderBase.h"
class FragmentShader :
	public aetherClass::ShaderBase
{
public:
	enum  Mode{
		eColor = 0,
		eTexture,
		eMixColor,
	};

	FragmentShader();
	~FragmentShader();
	Mode _property;
private:
	struct FragmentType{
		float _mode;
		aetherClass::Vector3 _Padding;
	};

	bool InitializeShader()override;
	void FinalizeShader()override;
	bool SetConstantBuffer()override;

	ID3D11Buffer* m_fragmentBuffer;

};

