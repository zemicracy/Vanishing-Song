#include "Cage.h"
#include <WorldReader.h>
using namespace aetherClass;
namespace{
	const Vector3 kCollideOffset = Vector3(0, -5, 0);
}
Cage::Cage(std::shared_ptr<GearFrame> gearframe, Vector3 position, ViewCamera* camera)
{
	mInitialize(gearframe,position, camera);

}


Cage::~Cage(){}

//
void Cage::mInitialize(std::shared_ptr<GearFrame> gearframe, Vector3 position, ViewCamera* camera){
	mRegisterGearHash(m_gearHash, gearframe);
	mLoadModel(gearframe, "data\\Player\\stay.aether");
	m_charaEntity.SetCamera(m_topGear, camera);
	mRegisterAnimation(60, "data\\Player\\stay.aether", "data\\Player\\wait.aether");
	m_animationCounter = NULL;
	m_changeAnimation = false;
	m_initialPosition = position;

	m_pCollider = std::make_shared<Cube>();
	m_pCollider->Initialize();

	m_pCollider->property._transform._translation = m_initialPosition + kCollideOffset;
	m_pCollider->property._color = Color(1.0, 0, 0, 0.3);
	m_pCollider->property._transform._scale = 10;
	m_pCollider->SetCamera(camera);
	return;
}

//
void Cage::mUpdate(const float timeScale, Vector3 position){
	mPrisonerAnimation();
	
	m_charaEntity.mFaceToObject(m_topGear, position);
}

//
void Cage::mRender(ShaderBase* tex, ShaderBase* color){
	m_charaEntity.mGearRender(m_topGear, tex, color);
}

//
void Cage::mFinalize(){
	m_pCollider->Finalize();
}

std::shared_ptr<Cube> Cage::mGetCollider(){
	return m_pCollider;
}

// �Ǘ����₷���悤�ɓo�^
void Cage::mRegisterGearHash(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>& hash, std::shared_ptr<GearFrame>&gearFrame){
	m_charaEntity.mRegisterParts(hash, Gear::eType::eBody, gearFrame->m_pBody);
	m_charaEntity.mRegisterParts(hash, Gear::eType::eWaist, gearFrame->m_pWaist);

	// ��
	m_charaEntity.mRegisterParts(hash, Gear::eType::eLeftArm, gearFrame->m_pLeftArm);
	m_charaEntity.mRegisterParts(hash, Gear::eType::eLeftFoot, gearFrame->m_pLeftFoot);
	// �E
	m_charaEntity.mRegisterParts(hash, Gear::eType::eRightArm, gearFrame->m_pRightArm);
	m_charaEntity.mRegisterParts(hash, Gear::eType::eRightFoot, gearFrame->m_pRightFoot);
}

//
void Cage::mLoadModel(std::shared_ptr<GearFrame>& gearFrame, std::string modelDataFile){
	WorldReader read;
	bool result = read.Load(modelDataFile.c_str());
	if (!result)
	{
		Debug::mErrorPrint("�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", __FILE__, __LINE__);
	}

	m_topGear = gearFrame->m_pBody;
	for (auto index : read.GetInputWorldInfo()._object){

		/*	��	*/
		if (index->_name == "Body"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pBody, index);
		}

		if (index->_name == "Waist"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pWaist, index);
		}

		/*	���㔼�g*/
		if (index->_name == "LeftArm"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftArm, index);
		}

		/*	�E�㔼�g	*/
		if (index->_name == "RightArm"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightArm, index);
		}

		/*	�E��	*/
		if (index->_name == "RightFoot"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightFoot, index);
		}

		/*	����	*/
		if (index->_name == "LeftFoot"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftFoot, index);
		}

	}

	read.UnLoad();

	return;
}

//
void Cage::mPrisonerAnimation(){
	// �ݒ肳��Ă��Ȃ��ꍇ�������Ȃ�
	Transform animationTransform;

	/*	�A�j���[�V�������s����	*/
	const int allFrame = m_stayAnimation._animationFrame;

	for (auto index : m_stayAnimation._animation)
	{
		// ��Ԃ̒l���擾
		animationTransform = m_charaEntity.mGetTransformInterpolation(index._start, index._end, allFrame, m_animationCounter);
		// ���]�p�A�̂������]������
		if (index._name == Gear::eType::eBody){
			animationTransform._scale._x = -1;
		}
		// �A�j���[�V�����̓K�p
		if (m_gearHash.find(index._name) != m_gearHash.end()){
			m_gearHash[index._name]->_pGear->property._transform = animationTransform;
		}
	}
	/*	�J�E���^�[�̏�Ԃ�؂�ւ���	*/
	if (m_animationCounter > allFrame){
		m_changeAnimation = true;
	}
	else if (m_animationCounter < NULL){
		m_changeAnimation = false;
	}

	// �J�E���^�[�̏�Ԃɍ��킹�ăt���[���J�E���g�̍X�V
	if (m_changeAnimation){
		m_animationCounter -= 1;
	}
	else{
		m_animationCounter += 1;
	}

	m_charaEntity.mGearMove(m_topGear, m_initialPosition);
	return;
}

//
void Cage::mRegisterAnimation(const int allFrame, std::string first, std::string last){
	AnimationFrame animation;
	bool result = false;

	result = m_charaEntity.mLoadAnimation(animation._animation, first, last);
	if (!result)
	{
		Debug::mErrorPrint("�ǂݍ��ݎ��s", __FILE__, __LINE__);
		return;
	}
	animation._animationFrame = allFrame;

	// �o�^
	m_stayAnimation = animation;
	return;
}