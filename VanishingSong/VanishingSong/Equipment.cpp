#include "Equipment.h"
#include "Debug.h"
#include "Const.h"
using namespace aetherClass;
Equipment::Equipment()
{
}

bool Equipment::mCreate(ViewCamera* camera, std::string path){
	bool result;
	m_pObject = std::make_shared<FbxModel>();
	result = m_pObject->LoadFBX(path, eAxisSystem::eAxisOpenGL);
	if (!result){
		Debug::mErrorPrint("FBXの読み込みに失敗", __FILE__, __LINE__);
		return false;
	}

	m_pObject->SetCamera(camera);
	m_pCollider = std::make_shared<Cube>();
	result = m_pCollider->Initialize();
	if (!result){
		Debug::mErrorPrint("コライダーの初期化に失敗", __FILE__, __LINE__);
		return false;
	}
	m_pCollider->property._color = Color(1.0f, 0.0f, 0.0f, 0.5f);
	m_pCollider->SetCamera(camera);
	return true;
}
void Equipment::mUpdate(const float timescale){
	// コライダーと武器の位置を常に一緒にする
	m_pCollider->property._transform._translation = m_pObject->property._transform._translation;

	mOnUpdate(timescale);
	return;
}
//
void Equipment::mRender(ShaderBase* shader){
	if (!m_pObject)return;
	m_pObject->Render(shader);
	if (kCharaDebug)
	{
		m_pCollider->Render(shader);
	}
	return;
}

//
std::shared_ptr<Cube> Equipment::mGetCollider(){
	return m_pCollider;
}

//
Transform& Equipment::mGetTransform(){
	return m_pObject->property._transform;
}

void Equipment::mDestroy(){
	if (m_pObject){
		m_pObject->Finalize();
		m_pObject.reset();
		m_pObject = nullptr;
	}

	if (m_pCollider){
		m_pCollider->Finalize();
		m_pCollider.reset();
		m_pCollider = nullptr;
	}

	return;
}