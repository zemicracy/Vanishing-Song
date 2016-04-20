#include "HalfFillShader.h"
#include"ModelUtility.h"
using namespace aetherClass;
using namespace aetherFunction;

HalfFillShader::HalfFillShader()
{
	m_fillTypeBuffer = nullptr;
}


HalfFillShader::~HalfFillShader()
{
}

bool HalfFillShader::InitializeShader(){
	HRESULT result;
	result = GetDirect3DManager()->GetDevice()->CreateBuffer(&GetConstantBufferDesc(sizeof(FillType)), NULL, &m_fillTypeBuffer);
	if (FAILED(result)){
		return false;
	}

	return true;
}
void HalfFillShader::FinalizeShader(){
	if (m_fillTypeBuffer){
		m_fillTypeBuffer->Release();
		m_fillTypeBuffer = nullptr;
	}
}
bool HalfFillShader::SetConstantBuffer(){
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mapped;
	FillType *dataptr;
	unsigned int bufferNumber;

	this->Exception();
	
	//ƒf[ƒ^‚Ì“o˜^
	result = GetDirect3DManager()->GetDeviceContext()->Map(m_fillTypeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (FAILED(result)){
		return false;
	}
	
	dataptr = (FillType*)mapped.pData;
	*dataptr = _property;
	GetDirect3DManager()->GetDeviceContext()->Unmap(m_fillTypeBuffer, 0);
	bufferNumber = 1;
	GetDirect3DManager()->GetDeviceContext()->PSSetConstantBuffers(bufferNumber, 1, &m_fillTypeBuffer);


	return true;
}

void HalfFillShader::Exception(){
	//‚O`‚P‚Ì”ÍˆÍ‚Éû‚ß‚é
	if (_property._interpolation < 0){
		_property._interpolation = 0;
	}
	else if (_property._interpolation > 1){
		_property._interpolation = 1;
	}
}