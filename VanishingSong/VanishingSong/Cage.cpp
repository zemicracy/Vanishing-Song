#include "Cage.h"
#include <WorldReader.h>
#include <Rectangle3D.h>
#include <GameClock.h>
#include "ResourceManager.h"
using namespace aetherClass;
namespace{
	const Vector3 kCollideOffset = Vector3(0, -5, 0);
}
Cage::Cage(std::shared_ptr<FbxModel> gearframe, Vector3 position, ViewCamera* camera, bool flg)
{
	mInitialize(gearframe,position, camera,flg);
	m_camera = camera;
	m_isMessage = false;
}

Cage::~Cage(){}

//
void Cage::mInitialize(std::shared_ptr<FbxModel> model, Vector3 position, ViewCamera* camera, bool isTought){
	m_model = model;
	m_model->SetCamera(camera);
	m_initialPosition = position;

	m_model->property._transform._translation = m_initialPosition;
	m_pCollider = std::make_shared<Cube>();
	m_pCollider->Initialize();

	m_pCollider->property._transform._translation = m_initialPosition + kCollideOffset;
	m_pCollider->property._color = Color(1.0, 0, 0, 0.3);
	m_pCollider->property._transform._scale = 10;
	m_pCollider->SetCamera(camera);

	m_commentFlame = std::make_shared<Rectangle3D>();
	m_commentFlame->Initialize();
	m_commentFlame->SetCamera(camera);
	m_commentFlame->SetTexture(ResourceManager::mGetInstance().GetTexture("commet").get());
	m_commentFlame->property._transform._translation = position + Vector3(0, 50, 0);
	m_commentFlame->property._transform._scale = Vector3(16, 12, 0);

	if (isTought){
		m_cage = std::make_shared<FbxModel>();
		m_cage->LoadFBX("Model\\Object\\Field\\ori.fbx", eAxisSystem::eAxisOpenGL);
		m_cage->SetTextureDirectoryName("Model\\Object\\Field\\tex");
		m_cage->SetCamera(camera);
		m_cage->property._transform._translation = position;
		m_cage->property._transform._scale = 3;
	}
	m_isTought = isTought;
	m_isComment = false;
	m_changeComment = false;
	m_changeCommentCount = NULL;
	return;
}

//
void Cage::mUpdate(const float timeScale, Vector3 position, const bool button){
	m_charaEntity.mFaceToObject(m_model, position);
	if (m_isComment){
		m_changeCommentCount += GameClock::GetDeltaTime();

		if (m_changeCommentCount> 1.0f){
			m_changeComment = !m_changeComment;
			m_changeComment = NULL;
		}

		if (m_changeComment){
			m_commentFlame->SetTexture(ResourceManager::mGetInstance().GetTexture("comment").get());
		}
		else{
			m_commentFlame->SetTexture(ResourceManager::mGetInstance().GetTexture("comment2").get());
		}

		if (button){
			m_isMessage = !m_isMessage;
		}
	}
}

//
void Cage::mRender(ShaderBase* tex, ShaderBase* color){
	if (m_isTought){
		m_cage->Render(tex);
	}
	m_model->Render(tex);

	if (m_isComment&&!m_isMessage){
		m_commentFlame->property._transform._rotation = m_camera->property._rotation;
		
			m_commentFlame->Render(tex);
	}
}

//
void Cage::mFinalize(){
	m_pCollider->Finalize();
}

std::shared_ptr<Cube> Cage::mGetCollider(){
	return m_pCollider;
}

void Cage::mSetIsTought(bool flg){
	m_isTought = flg;
}

//
void Cage::mSetIsComment(bool flg){
	m_isComment = flg;
}

//
Vector3 Cage::mGetPosition(){
	return m_model->property._transform._translation;
}

//
bool Cage::mGetMessageRun(){
	return m_isMessage;
}