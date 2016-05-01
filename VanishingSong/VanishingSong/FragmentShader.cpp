#include "FragmentShader.h"
#include"ModelUtility.h"

using namespace aetherClass;

FragmentShader::FragmentShader()
{
	_property = Mode::eColor;
}


FragmentShader::~FragmentShader()
{
}

bool FragmentShader::InitializeShader(){
	HRESULT result;
	result = GetDirect3DManager()->GetDevice()->CreateBuffer(&aetherFunction::GetConstantBufferDesc(sizeof(FragmentType)), NULL, &m_fragmentBuffer);
	if (FAILED(result)){
		return false;
	}
	return true;
}
void FragmentShader::FinalizeShader(){
	m_fragmentBuffer->Release();
}
bool FragmentShader::SetConstantBuffer(){
	auto d3d = GetDirect3DManager()->GetDeviceContext();
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mapped;
	FragmentType *dataptr;
	unsigned int bufferNumber;

	result = GetDirect3DManager()->GetDeviceContext()->Map(m_fragmentBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (FAILED(result)){
		return false;
	}

	dataptr = (FragmentType*)mapped.pData;
	dataptr->_mode = (float)_property;
	GetDirect3DManager()->GetDeviceContext()->Unmap(m_fragmentBuffer, 0);
	bufferNumber = 1;
	GetDirect3DManager()->GetDeviceContext()->PSSetConstantBuffers(bufferNumber, 1, &m_fragmentBuffer);

}

