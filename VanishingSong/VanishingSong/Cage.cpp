#include "Cage.h"
#include <WorldReader.h>
using namespace aetherClass;
namespace{
	const Vector3 kCollideOffset = Vector3(0, -5, 0);
}
Cage::Cage(std::shared_ptr<FbxModel> gearframe, Vector3 position, ViewCamera* camera, bool flg)
{
	mInitialize(gearframe,position, camera,flg);

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

	if (isTought){
		m_cage = std::make_shared<FbxModel>();
		m_cage->LoadFBX("ori", eAxisSystem::eAxisOpenGL);
		m_cage->SetTextureDirectoryName("deirectory");
		m_cage->SetCamera(camera);
		m_cage->property._transform._translation = position;
	}
	m_isTought = isTought;
	return;
}

//
void Cage::mUpdate(const float timeScale, Vector3 position){
	m_charaEntity.mFaceToObject(m_model, position);
}

//
void Cage::mRender(ShaderBase* tex, ShaderBase* color){
	if (m_isTought){
		m_cage->Render(tex);
	}
	m_model->Render(tex);
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
