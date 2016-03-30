#include "Player.h"
#include "Debug.h"
using namespace aetherClass;
namespace{
	const bool kPlayerDebug = true;
}
Player::Player()
{
	m_pGearFrame = nullptr;
	m_actionHash.clear();
	m_pAction = nullptr;
}


Player::~Player()
{
}


bool Player::mInitialize(ViewCamera* camera){
	m_pGearFrame = std::make_shared<GearFrame>();

	// �̂̃p�[�c
	m_pGearFrame->m_pBody = mSetUpGear("null", Gear::eType::eBody, camera);

	// ���̃p�[�c
	m_pGearFrame->m_pWaist = mSetUpGear("null", Gear::eType::eWaist, camera);

	//���̃p�[�c
	m_pGearFrame->m_pRightShoulder = mSetUpGear("null", Gear::eType::eRightShourlder, camera);
	m_pGearFrame->m_pLeftShoulder = mSetUpGear("null", Gear::eType::eLeftShourlder, camera);

	// �r�̃p�[�c
	m_pGearFrame->m_pLeftUpperArm = mSetUpGear("null", Gear::eType::eLeftUpperArm, camera);
	m_pGearFrame->m_pRightUpperArm = mSetUpGear("null", Gear::eType::eRightUpperArm, camera);
	m_pGearFrame->m_pLeftLowerArm = mSetUpGear("null", Gear::eType::eLeftLowerArm, camera);
	m_pGearFrame->m_pRightLowerArm = mSetUpGear("null", Gear::eType::eRightLowerArm, camera);

	// ��̃p�[�c
	m_pGearFrame->m_pLeftHand = mSetUpGear("null", Gear::eType::eLeftHand, camera);
	m_pGearFrame->m_pRightHand= mSetUpGear("null", Gear::eType::eRightHand, camera);

	// ���̃p�[�c
	m_pGearFrame->m_pLeftUpperLeg = mSetUpGear("null", Gear::eType::eLeftUpperLeg, camera);
	m_pGearFrame->m_pRightUpperLeg = mSetUpGear("null", Gear::eType::eRightUpperLeg, camera);
	m_pGearFrame->m_pLeftLowerLeg = mSetUpGear("null", Gear::eType::eLeftLowerLeg, camera);
	m_pGearFrame->m_pRightLowerLeg = mSetUpGear("null", Gear::eType::eRightLowerLeg, camera);

	// �̂Ƀp�[�c�Ƃ̐e�q�֌W
	mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pWaist);
	mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pRightShoulder);
	mCreateRelationship(m_pGearFrame->m_pBody, m_pGearFrame->m_pLeftShoulder);

	// �E�r�̐e�q�֌W
	mCreateRelationship(m_pGearFrame->m_pRightShoulder, m_pGearFrame->m_pRightUpperArm);
	mCreateRelationship(m_pGearFrame->m_pRightUpperArm, m_pGearFrame->m_pRightLowerArm);
	mCreateRelationship(m_pGearFrame->m_pRightLowerArm,m_pGearFrame->m_pRightHand);

	// ���r�̐e�q�֌W
	mCreateRelationship(m_pGearFrame->m_pLeftShoulder, m_pGearFrame->m_pLeftUpperArm);
	mCreateRelationship(m_pGearFrame->m_pLeftUpperArm, m_pGearFrame->m_pLeftLowerArm);
	mCreateRelationship(m_pGearFrame->m_pLeftLowerArm, m_pGearFrame->m_pLeftHand);

	// �E���̐e�q�֌W
	mCreateRelationship(m_pGearFrame->m_pWaist, m_pGearFrame->m_pRightUpperLeg);
	mCreateRelationship(m_pGearFrame->m_pRightUpperLeg, m_pGearFrame->m_pRightLowerLeg);

	// �����̐e�q�֌W
	mCreateRelationship(m_pGearFrame->m_pWaist, m_pGearFrame->m_pLeftUpperLeg);
	mCreateRelationship(m_pGearFrame->m_pLeftUpperLeg, m_pGearFrame->m_pLeftLowerLeg);

	if (kPlayerDebug)
	{
		Debug::mPrint("�v���C���[���f�o�b�O���[�h�ł�");
	}
	return true;
}

//
void Player::mUpdate(const float timeScale){
	
	return;
}

//
void Player::mRender(aetherClass::ShaderBase* shader){
	
	if (!m_pGearFrame->m_pBody)return;

	// �S�Ă̐e�͑̂̃p�[�c�Ȃ̂ŁA�K���̂̃p�[�c����n�߂�
	mGearRender(m_pGearFrame->m_pBody, shader);
}

//
void Player::mAction(std::shared_ptr<ActionCommand> command){
	return;
}

/*
	�p�[�c��`�悷��֐�
	�����ɂ͕`�悷��p�[�c��n��
*/
void Player::mGearRender(std::shared_ptr<Gear> gear, aetherClass::ShaderBase* shader){
	// �f�o�b�O���[�h�̎��̓R���C�_�[�̕\��
	if (kPlayerDebug)
	{
		gear->_pColider->Render(shader);
	}

	// ������������ɏI����Ă��Ȃ��̂Ȃ牽�����Ȃ�
	if (!gear || !gear->_pGear)return;

	gear->_pGear->Render(shader);

	// �q��������΂��̕������ċA
	for (auto child : gear->_pChildren){
		mGearRender(child, shader);
	}

	return;
}

/*
	�p�[�c�ЂƂЂƂ̏������p
	���F���f���̃p�X
	���F����������p�[�c�̎��
	��O�F�J�����̃|�C���^
*/
std::shared_ptr<Gear> Player::mSetUpGear(std::string path, Gear::eType gearType,aetherClass::ViewCamera* view){
	std::shared_ptr<Gear> pGear;
	bool success;

	// �����̃p�[�c�̏�����
	pGear = std::make_shared<Gear>();
	pGear->_pGear = std::make_shared<FbxModel>();
	// ���g�̎�ނ̐ݒ�
	pGear->_type = gearType;

	// ���f���̓ǂݎ��
	success = pGear->_pGear->LoadFBX(path, eAxisSystem::eAxisOpenGL);
	if (!success)
	{
		Debug::mErrorPrint("FBX�̏�����������ɍs���܂���ł����B", __FILE__, __LINE__,Debug::eState::eWindow);

		if (kPlayerDebug)
		{
			// �R���C�_�[�̏�����
			pGear->_pColider = std::make_shared<Cube>();
			pGear->_pColider->Initialize();
			// �R���C�_�[�̐F��Ԃɐݒ�
			pGear->_pColider->property._color = Color(1.0f, 0.0f, 0.0f, 0.3f);
			pGear->_pColider->SetCamera(view);
			return pGear;
		}
		return nullptr;
	}

	pGear->_pGear->SetCamera(view);
	// �e�N�X�`���̓ǂݍ���
	//pGear->_pGear->SetTextureDirectoryName("texture")

	// �R���C�_�[�̏�����
	pGear->_pColider = std::make_shared<Cube>();
	pGear->_pColider->Initialize();
	// �R���C�_�[�̐F��Ԃɐݒ�
	pGear->_pColider->property._color = Color(1.0f, 0.0f, 0.0f, 0.3f);
	pGear->_pColider->SetCamera(view);
	return pGear;
}

/*
	�e�q�֌W���쐬����֐�
	�������F�e�ɂȂ�|�C���^
	�������F�q���ɂȂ�|�C���^
*/
void Player::mCreateRelationship(std::shared_ptr<Gear> parentGear, std::shared_ptr<Gear> child){
	parentGear->_pChildren.push_back(child);
	/*	�p�[�c��A�����鏈��	*/
	child->_pParent = parentGear;
}