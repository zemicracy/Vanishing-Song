#include "Player.h"
#include "Debug.h"
#include "Utility.h"
#include <MathUtility.h>
#include <GameController.h>
#include <WorldReader.h>
#include <GameClock.h>
using namespace aetherClass;

Player::Player()
{
	m_pGearFrame = nullptr;
	m_pTopGear = nullptr;
	m_pActionCommand = nullptr;
}


Player::~Player()
{
	mFinalize();
}

//
bool Player::mInitialize(){
	if (kCharaDebug)
	{
		Debug::mPrint("�v���C���[ �f�o�b�O���[�h�ł�");
		Debug::mPrint("�v���C���[ ���������J�n���܂�");
		Debug::mPrint("");
	}

	mFinalize();

	m_playerView.property._translation = Vector3(0, 0, -20);
	// �M�A�n�̏������p
	mInitializeGear(m_pGearFrame, &m_playerView);

	// �p�[�c�̏����ʒu
	mLoadModelProperty(m_pGearFrame, "data\\PlayerModel.aether");

	if (kCharaDebug)
	{
		Debug::mPrint("�v���C���[ �������I�����܂���");
		Debug::mPrint("");
	}

	m_cameraOffset._translation = m_playerView.property._translation;
	m_cameraOffset._rotation = m_playerView.property._rotation;

	m_cameraRotation = kVector3Zero;

	// �����ʒu�̐ݒ�
	m_charaEntity.mGearMove(m_pTopGear, Vector3(0, 20, 0));

	mInitializeCollider(m_pCubeCollider);
	return true;
}

/*
�������
*/
void Player::mFinalize(){
	if (m_pGearFrame)
	{
		m_pGearFrame->Release();
		m_pGearFrame.reset();
		m_pGearFrame = nullptr;
	}

	// 
	if (m_pTopGear)
	{
		m_pTopGear->Release();
		m_pTopGear.reset();
		m_pTopGear = nullptr;
	}

	// �A�N�V�����R�}���h
	if (m_pActionCommand)
	{
		m_pActionCommand.reset();
		m_pActionCommand = nullptr;
	}

	if (m_pCubeCollider)
	{
		m_pCubeCollider->Finalize();
		m_pCubeCollider.reset();
		m_pCubeCollider = nullptr;
	}

	// �X�e�[�^�X�̃��Z�b�g
	m_status.Reset();

	m_prevCommand = eCommandType::eNull;
	m_state = Player::eState::eNull;
	m_actionCount = NULL;
	return;
}


/*
�v���C���[�̍X�V����
*/
void Player::mUpdate(const float timeScale){

	// �ړ��Ɏg���l�̂��擾
	Transform transform = mReadKey(timeScale);

	// �ړ��ɕω��ʂ������
	if (transform._translation == kVector3Zero){
		Debug::mPrint("WAIT");
		
		m_state = eState::eWait;
	}
	else{
		Debug::mPrint("MOVE");
		m_state = eState::eMove;
	}

	// ���ۂ̈ړ�����
	m_charaEntity.mGearMove(m_pTopGear, transform._translation);
	
	// �R���C�_�[��������
	m_pCubeCollider->property._transform._translation += transform._translation;
	
	if (GameController::GetKey().IsKeyDown('Q'))
	{
		m_cameraRotation._y += 1.03f;
		
	}
	else if (GameController::GetKey().IsKeyDown('E')){
		m_cameraRotation._y -= 1.03f;
		
	}
	
	auto gearTranslation = m_pTopGear->_pGear->property._transform._translation;

	Matrix4x4 rotationMatrix;
	rotationMatrix.PitchYawRoll(m_cameraRotation*kAetherRadian);
	Vector3 position = transform._translation + m_cameraOffset._translation;
	position = position.TransformCoordNormal(rotationMatrix);

	m_playerView.property._translation = gearTranslation+position;
	m_playerView.property._rotation = m_cameraRotation + m_cameraOffset._rotation;

	return;
}

//
void Player::mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader){

	if (!m_pTopGear)return;

	if (kCharaDebug)
	{
		m_pCubeCollider->Render(colliderShader);
	}

	m_playerView.Render();

	// �S�Ă̐e�͑̂̃p�[�c�Ȃ̂ŁA�K���̂̃p�[�c����n�߂�
	m_charaEntity.mGearRender(m_pTopGear, modelShader, colliderShader);

	return;
}

//
eCommandType Player::mCommand(std::shared_ptr<ActionCommand> command, const float timeScale){

	// ������s���A�N�V�������擾
	m_status._nowCommand = command->mGetType();

	// �O��ƈႦ�Ύ��s����0�ɂ���
	if (m_status._nowCommand != m_prevCommand){
		m_actionCount = kZeroPoint;
		Debug::mPrint("Change Action");
	}

	// �A�N�V�����̎��s
	command->mAction(m_pGearFrame, timeScale,m_actionCount);

	if (GameController::GetKey().KeyDownTrigger('F'))
	{
		Debug::mPrint("Run Action :" + std::to_string(m_actionCount) + "���");
	}
	
	m_actionCount += 1;

	// ��Ԃ��㏑��
	m_prevCommand = m_status._nowCommand;

	return m_prevCommand;
}

/*
�s�������A�N�V������o�^
�������F�A�N�V�����̃^�C�v
�������F���Ԗڂ̃A�N�V������
*/
void Player::mAddPrevActionCmmand(eCommandType action, const int id){
	m_status._prevActionList[id] = action;

	return;
}

/*
NULL�Ŗ��ߐs����
*/
void Player::mResetPrevActionList(){
	m_status._prevActionList.fill(eCommandType::eNull);

	return;
}

//
aetherClass::ViewCamera *Player::mGetView(){
	return &m_playerView;
}

std::shared_ptr<Cube> Player::mGetColldier(){
	return m_pCubeCollider;
}

/*
�M�A�n�̏��������܂Ƃ߂�����

*/
bool Player::mInitializeGear(std::shared_ptr<GearFrame>& gearFrame, aetherClass::ViewCamera* camera){
	gearFrame = std::make_shared<GearFrame>();

	// �̂̃p�[�c
	gearFrame->m_pBody = m_charaEntity.mSetUpGear("Model\\Player\\body.fbx", Gear::eType::eBody, camera);

	// ���̃p�[�c
	gearFrame->m_pWaist = m_charaEntity.mSetUpGear("Model\\Player\\waist.fbx", Gear::eType::eWaist, camera);

	// �r�̃p�[�c
	gearFrame->m_pLeftUpperArm = m_charaEntity.mSetUpGear("Model\\Player\\arm1.fbx", Gear::eType::eLeftUpperArm, camera);
	gearFrame->m_pRightUpperArm = m_charaEntity.mSetUpGear("Model\\Player\\arm1.fbx", Gear::eType::eRightUpperArm, camera);
	gearFrame->m_pLeftLowerArm = m_charaEntity.mSetUpGear("Model\\Player\\arm2.fbx", Gear::eType::eLeftLowerArm, camera);
	gearFrame->m_pRightLowerArm = m_charaEntity.mSetUpGear("Model\\Player\\arm2.fbx", Gear::eType::eRightLowerArm, camera);

	// ��̃p�[�c
	gearFrame->m_pLeftHand = m_charaEntity.mSetUpGear("Model\\Player\\hand.fbx", Gear::eType::eLeftHand, camera);
	gearFrame->m_pRightHand = m_charaEntity.mSetUpGear("Model\\Player\\hand.fbx", Gear::eType::eRightHand, camera);

	// ���̃p�[�c
	gearFrame->m_pLeftUpperLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg1.fbx", Gear::eType::eLeftUpperLeg, camera);
	gearFrame->m_pRightUpperLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg1.fbx", Gear::eType::eRightUpperLeg, camera);
	gearFrame->m_pLeftLowerLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg2.fbx", Gear::eType::eLeftLowerLeg, camera);
	gearFrame->m_pRightLowerLeg = m_charaEntity.mSetUpGear("Model\\Player\\leg2.fbx", Gear::eType::eRightLowerLeg, camera);

	// �ŏ�ʂɓ�����p�[�c�̐ݒ�
	m_pTopGear = gearFrame->m_pBody;

	// �̂Ƀp�[�c�Ƃ̐e�q�֌W
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pWaist);
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pRightUpperArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pLeftUpperArm);

	// �E�r�̐e�q�֌W
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightUpperArm, gearFrame->m_pRightLowerArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightLowerArm, gearFrame->m_pRightHand);

	// ���r�̐e�q�֌W
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftUpperArm, gearFrame->m_pLeftLowerArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftLowerArm, gearFrame->m_pLeftHand);

	// �E���̐e�q�֌W
	m_charaEntity.mCreateRelationship(gearFrame->m_pWaist, gearFrame->m_pRightUpperLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightUpperLeg, gearFrame->m_pRightLowerLeg);

	// �����̐e�q�֌W
	m_charaEntity.mCreateRelationship(gearFrame->m_pWaist, gearFrame->m_pLeftUpperLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftUpperLeg, gearFrame->m_pLeftLowerLeg);

	return true;
}

/*

*/
bool Player::mLoadModelProperty(std::shared_ptr<GearFrame>& gearFrame, std::string modelDataFile){
	WorldReader read;
	bool result = read.Load(modelDataFile.c_str());
	if (!result)
	{
		Debug::mErrorPrint("�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", __FILE__, __LINE__);
		return false;
	}

	for (auto index : read.GetInputWorldInfo()._object){

		/*	��	*/
		if (index->_name == "Body"){
			SetLoadModelValue(gearFrame->m_pBody, index);
		}

		if (index->_name == "Waist"){
			SetLoadModelValue(gearFrame->m_pWaist, index);
		}

		/*	���㔼�g*/
		if (index->_name == "LeftUpperArm"){
			SetLoadModelValue(gearFrame->m_pLeftUpperArm, index);
		}

		if (index->_name == "LeftLowerArm"){
			SetLoadModelValue(gearFrame->m_pLeftLowerArm, index);
		}

		if (index->_name == "LeftHand"){
			SetLoadModelValue(gearFrame->m_pLeftHand, index);
		}

		/*	�E�㔼�g	*/
		if (index->_name == "RightUpperArm"){
			SetLoadModelValue(gearFrame->m_pRightUpperArm, index);
		}

		if (index->_name == "RightLowerArm"){
			SetLoadModelValue(gearFrame->m_pRightLowerArm, index);
		}

		if (index->_name == "RightHand"){
			SetLoadModelValue(gearFrame->m_pRightHand, index);
		}

		/*	�E��	*/
		if (index->_name == "RightUpperLeg"){
			SetLoadModelValue(gearFrame->m_pRightUpperLeg, index);
		}

		if (index->_name == "RightLowerLeg"){
			SetLoadModelValue(gearFrame->m_pRightLowerLeg, index);
		}

		/*	����	*/
		if (index->_name == "LeftUpperLeg"){
			SetLoadModelValue(gearFrame->m_pLeftUpperLeg, index);
		}

		if (index->_name == "LeftLowerLeg"){
			SetLoadModelValue(gearFrame->m_pLeftLowerLeg, index);
		}

	}

	// �J�����̏�����
	m_playerView.property._translation = read.GetInputWorldInfo()._camera._position;
	m_playerView.property._rotation = read.GetInputWorldInfo()._camera._rotation;
	
	read.UnLoad();

	return true;
}

void Player::SetLoadModelValue(std::shared_ptr<Gear>& gear, ObjectInfo* info){

	gear->_pGear->property._transform = info->_transform;

	if (gear->_pParent)
	{
		std::shared_ptr<Gear> pParent = gear->_pParent;
		// �ŏ�ʂƂ̍�
		gear->_topDifference._translation = gear->_pGear->property._transform._translation - m_pTopGear->_pGear->property._transform._translation;
		gear->_topDifference._rotation = gear->_pGear->property._transform._rotation - m_pTopGear->_pGear->property._transform._rotation;

		// �e�Ƃ̍�
		gear->_parentDifference._translation = gear->_pGear->property._transform._translation - m_pTopGear->_pGear->property._transform._translation;
		gear->_parentDifference._rotation = gear->_pGear->property._transform._rotation - m_pTopGear->_pGear->property._transform._rotation;
	}

	return;
}

/*
�L�[��ǂݍ���
�Ԃ�͍��̂Ƃ���transform
*/
Transform Player::mReadKey(const float timeScale){

	Transform transform;

	// ���s�̈ړ�(Z��)
	if (GameController::GetKey().IsKeyDown('W')){
		transform._translation._z = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('S')){
		transform._translation._z = -(GameClock::GetDeltaTime()*timeScale);
	}

	// ���̈ړ�(X��)
	if (GameController::GetKey().IsKeyDown('D')){
		transform._translation._x = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('A')){
		transform._translation._x = -(GameClock::GetDeltaTime()*timeScale);
	}

	// �L�������f�o�b�O���[�h����Ȃ��Ȃ炱���ŏI��
	if (!kCharaDebug) return transform;

	// ��]�p(Y��)
	/*if (GameController::GetKey().IsKeyDown('Q')){
		transform._rotation._y = GameClock::GetDeltaTime()*timeScale * 100;
	}
	else if (GameController::GetKey().IsKeyDown('E')){
		transform._rotation._y = -(GameClock::GetDeltaTime()*timeScale * 100);
	}*/

	// �f�o�b�O�p
	Vector3 debug = m_pTopGear->_pGear->property._transform._translation;
	if (GameController::GetKey().KeyDownTrigger('P')){
		Debug::mPrint("---Player�̌��݂̍��W---");
		Debug::mPrint("X :" + std::to_string(debug._x));
		Debug::mPrint("Y :" + std::to_string(debug._y));
		Debug::mPrint("Z :" + std::to_string(debug._z));
		Debug::mPrint("------------------------");
	}

	return transform;
}

void Player::mInitializeCollider(std::shared_ptr<aetherClass::Cube>& collider){
	collider = std::make_shared<Cube>();
	collider->Initialize();
	collider->property._transform._translation = m_pTopGear->_pGear->property._transform._translation + Vector3(1, -5, -0);
	collider->property._transform._scale = 10;
	collider->property._color = Color(1, 0, 0, 0.5);
	collider->SetCamera(&m_playerView);
}


