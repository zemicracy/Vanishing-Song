#include "Bullet.h"
#include "Const.h"
using namespace aetherClass;
Bullet::Bullet()
{
}


Bullet::~Bullet()
{
	mDestroy();
}

//
bool Bullet::mCreate(std::string path){
	bool result;
	m_pBullet = std::make_shared<FbxModel>();
	result = m_pBullet->LoadFBX(path, eAxisSystem::eAxisOpenGL);
	if (!result){
		Debug::mErrorPrint("FBXの読み込みに失敗", __FILE__, __LINE__);
		return false;
	}

	m_pCollider = std::make_shared<Cube>();
	result = m_pCollider->Initialize();
	if (!result){
		Debug::mErrorPrint("コライダーの初期化に失敗", __FILE__, __LINE__);
		return false;
	}
	m_pCollider->property._color = Color(1.0f, 0.0f, 0.0f, 0.5f);
	return true;
}

//
void Bullet::mUpdate(const float timeScale){
	if (!m_pBullet)return;

}

//
void Bullet::mRender(ShaderBase* shader){
	if (!m_pBullet)return;
	m_pBullet->Render(shader);
	if (kCharaDebug)
	{
		m_pCollider->Render(shader);
	}
	return;
}

//
std::shared_ptr<Cube> Bullet::mGetCollider(){
	return m_pCollider;
}

//
Vector3& Bullet::mGetTranslation(){
	return m_pBullet->property._transform._translation;
}

//
Vector3& Bullet::mGetRotation(){
	return m_pBullet->property._transform._rotation;
}

void Bullet::mDestroy(){
	if (m_pBullet){
		m_pBullet->Finalize();
		m_pBullet.reset();
		m_pBullet = nullptr;
	}

	if (m_pCollider){
		m_pCollider->Finalize();
		m_pCollider.reset();
		m_pCollider = nullptr;
	}

	return;
}