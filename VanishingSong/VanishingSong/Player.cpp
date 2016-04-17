#include "Player.h"
#include "Debug.h"
#include "Utility.h"
#include <MathUtility.h>
#include <GameController.h>
#include <WorldReader.h>
#include <GameClock.h>
using namespace aetherClass;

namespace{
	const int kWaitAnimationFrame = 60;
	const int kMoveAnimationFrame = 5;
}

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
	bool result;
	if (kCharaDebug)
	{
		Debug::mPrint("�v���C���[ �f�o�b�O���[�h�ł�");
		Debug::mPrint("�v���C���[ ���������J�n���܂�");
	}

	mFinalize();

	m_playerView.property._translation = Vector3(0, 0, -20);
	// �M�A�n�̏������p
	result = mInitializeGear(m_pGearFrame, &m_playerView);
	if (!result)
	{
		Debug::mErrorPrint("�M�A�̏������Ɏ��s", __FILE__, __LINE__);
		return false;
	}
	// �p�[�c�̏����ʒu
	result = mLoadModelProperty(m_pGearFrame, "data\\PlayerDefault.aether");
	if (!result)
	{
		Debug::mErrorPrint("�M�A�̈ʒu���f�Ɏ��s", __FILE__, __LINE__);
		return false;
	}

	m_cameraOffset._translation = m_playerView.property._translation;
	m_cameraOffset._rotation = m_playerView.property._rotation;

	m_cameraRotation = kVector3Zero;

	// �����ʒu�̐ݒ�
	m_moveTransform._translation= Vector3(0,20,0);

	// �A�z�z��ɓo�^
	mRegisterParts(m_pGearPartsHash, Gear::eType::eBody, m_pGearFrame->m_pBody);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eWaist, m_pGearFrame->m_pWaist);

	// ��
	mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftHand, m_pGearFrame->m_pLeftHand);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftLowerArm, m_pGearFrame->m_pLeftLowerArm);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftLowerLeg, m_pGearFrame->m_pLeftLowerLeg);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftUpperArm, m_pGearFrame->m_pLeftUpperArm);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eLeftUpperLeg, m_pGearFrame->m_pLeftUpperLeg);

	// �E
	mRegisterParts(m_pGearPartsHash, Gear::eType::eRightHand, m_pGearFrame->m_pRightHand);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eRightLowerArm, m_pGearFrame->m_pRightLowerArm);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eRightLowerLeg, m_pGearFrame->m_pRightLowerLeg);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eRightUpperArm, m_pGearFrame->m_pRightUpperArm);
	mRegisterParts(m_pGearPartsHash, Gear::eType::eRightUpperLeg, m_pGearFrame->m_pRightUpperLeg);

	// �R���C�_�[�̏�����
	mSetUpCollider(m_pCubeCollider, m_pTopGear->_pGear->property._transform._translation, Vector3(1, -5, -0));

	/*	��{�I�ȃA�j���[�V�����̓o�^	*/
	mRegisterAnimation(Player::eState::eMove, kMoveAnimationFrame,"data\\PlayerDefault.aether", "data\\PlayerMove.aether");
	mRegisterAnimation(Player::eState::eWait, kWaitAnimationFrame,"data\\PlayerDefault.aether", "data\\PlayerWait.aether");
	if (kCharaDebug)
	{
		Debug::mPrint("�v���C���[ �������I�����܂���");
	}
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
	m_defaultAnimation.clear();

	m_prevCommand = eCommandType::eNull;
	m_prevState = eState::eNull;

	return;
}


/*
�v���C���[�̍X�V����
*/
void Player::mUpdate(const float timeScale){
	
	Transform animTransform;

	// �ړ��Ɏg���l�̂��擾
	m_moveTransform._translation += mReadKey(timeScale)._transform._translation;
	m_moveTransform._rotation += mReadKey(timeScale)._transform._rotation;
	
	eState state = mReadKey(timeScale)._state;
	// ��{�I�ȃA�j���[�V�����̍Đ�
	mGetAnimationTransform(state, animTransform);


	// ���ۂ̈ړ�����
	animTransform._translation += m_moveTransform._translation;
	animTransform._rotation+= m_moveTransform._rotation;

	m_charaEntity.mGearMove(m_pTopGear, animTransform._translation);
	m_charaEntity.mTopGearRotation(m_pTopGear, m_moveTransform._rotation);

	// �R���C�_�[�͏��Body�Ɠ����ɂ��Ă���
	m_pCubeCollider->property._transform._translation = m_pGearFrame->m_pBody->_pGear->property._transform._translation;
	

	/*if (GameController::GetKey().IsKeyDown('Q'))
	{
		m_cameraRotation._y += 1.03f;
		
	}
	else if (GameController::GetKey().IsKeyDown('E')){
		m_cameraRotation._y -= 1.03f;
		
	}*/
	mLookAtView(m_playerView,m_cameraRotation,m_pTopGear->_pGear->property._transform._translation);
	
	return;
}

//
void Player::mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader){

	if (!m_pTopGear)return;
	m_playerView.Render();

	if (kCharaDebug)
	{
	//	m_pCubeCollider->Render(modelShader);
	}

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
		m_actionCount._commandFrame = kZeroPoint;
		Debug::mPrint("Change Action");
	}

	// �A�N�V�����̎��s
	command->mAction(m_pGearFrame, timeScale,m_actionCount._commandFrame);

	if (GameController::GetKey().KeyDownTrigger('F'))
	{
		Debug::mPrint("Run Action :" + std::to_string(m_actionCount._commandFrame) + "���");
	}
	
	m_actionCount._commandFrame += 1;

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
Player::TransformState Player::mReadKey(const float timeScale){

	TransformState state;

	// ���s�̈ړ�(Z��)
	if (GameController::GetKey().IsKeyDown('W')){
		state._transform._translation._z = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('S')){
		state._transform._translation._z = -(GameClock::GetDeltaTime()*timeScale);
	}

	// ���̈ړ�(X��)
	if (GameController::GetKey().IsKeyDown('D')){
		state._transform._translation._x = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('A')){
		state._transform._translation._x = -(GameClock::GetDeltaTime()*timeScale);
	}

	// �ړ��������
	if (state._transform._translation == kVector3Zero){
		state._state = eState::eWait;
	}
	else{
		state._state = eState::eMove;
	}

	// �L�������f�o�b�O���[�h����Ȃ��Ȃ炱���ŏI��
	if (!kCharaDebug) return state;

	// ��]�p(Y��)
	if (GameController::GetKey().IsKeyDown('Q')){
		state._transform._rotation._y = GameClock::GetDeltaTime()*timeScale * 1;
	}
	else if (GameController::GetKey().IsKeyDown('E')){
		state._transform._rotation._y = -(GameClock::GetDeltaTime()*timeScale * 1);
	}

	// �f�o�b�O�p
	Vector3 debug = m_pTopGear->_pGear->property._transform._translation;
	if (GameController::GetKey().KeyDownTrigger('P')){
		Debug::mPrint("---Player�̌��݂̍��W---");
		Debug::mPrint("X :" + std::to_string(debug._x));
		Debug::mPrint("Y :" + std::to_string(debug._y));
		Debug::mPrint("Z :" + std::to_string(debug._z));
		Debug::mPrint("------------------------");
	}

	return state;
}

/*
*/
void Player::mSetUpCollider(std::shared_ptr<aetherClass::Cube>& collider, Vector3 original, Vector3 offset){
	collider = std::make_shared<Cube>();
	collider->Initialize();
	collider->property._transform._translation = original + offset;
	collider->property._transform._scale = 10;
	collider->property._color = Color(1, 0, 0, 0.5);
	collider->SetCamera(&m_playerView);
	return;
}

/*
*/
void Player::mRegisterAnimation(Player::eState key, const int allFrame, std::string first, std::string last){
	AnimationFrame animation;
	bool result = false;

	// ���ɂ���o�^�ς݂Ȃ牽�����Ȃ�
	if (m_defaultAnimation.find(key) != m_defaultAnimation.end()) return;

	result = m_charaEntity.mLoadAnimation(animation._animation, first, last);
	if (!result)
	{
		Debug::mErrorPrint("�ǂݍ��ݎ��s", __FILE__, __LINE__);
		return;
	}
	animation._animationFrame = allFrame;

	// �o�^
	m_defaultAnimation.insert(std::make_pair(key, animation));

	return;
}

/*
*/
void Player::mGetAnimationTransform(Player::eState m_state, Transform transform){

	// �O��ƈႤ�Ƃ��͍X�V
	if (m_prevState != m_state){
		m_actionCount._defaultFrame = NULL;
		m_prevState = m_state;
	}

	Transform animationTransform;

	if (m_defaultAnimation.find(m_state) == m_defaultAnimation.end()) return;

	/*	�A�j���[�V�������s����	*/
	const int allFrame = m_defaultAnimation[m_state]._animationFrame;
	for (auto index : m_defaultAnimation[m_state]._animation)
	{
		animationTransform = m_charaEntity.mGetTransformInterpolation(index._start, index._end,allFrame , m_actionCount._defaultFrame);

		animationTransform._translation += transform._translation;
		animationTransform._rotation += transform._rotation;
		if (m_pGearPartsHash.find(index._name) != m_pGearPartsHash.end()){
			m_pGearPartsHash[index._name]->_pGear->property._transform = animationTransform;
		}
	}

	
	if (m_state == eState::eWait){
		/*	�J�E���^�[�̏�Ԃ�؂�ւ���	*/
		if (m_actionCount._defaultFrame > kWaitAnimationFrame){
			m_actionCount._changeDefaultFrame = true;
		}
		else if (m_actionCount._defaultFrame < kZeroPoint){
			m_actionCount._changeDefaultFrame = false;
		}

		// �J�E���^�[�̏�Ԃɍ��킹�ăt���[���J�E���g�̍X�V
		if (m_actionCount._changeDefaultFrame){
			m_actionCount._defaultFrame -= 1;
		}
		else{
			m_actionCount._defaultFrame += 1;
		}

	}
	else{
		m_actionCount._defaultFrame += 1;
	}

	return;
}


void Player::mRegisterParts(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>& hash,Gear::eType type, std::shared_ptr<Gear>& parts){

	// �o�^�ς݂Ȃ牽�����Ȃ�
	if (hash.find(type) != hash.end() || !parts)return;

	hash.insert(std::make_pair(type, parts));
	return;
}


void Player::mLookAtView(ViewCamera& view,Vector3 rotation,Vector3 lookAtPosition){
	
	Matrix4x4 rotationMatrix;
	rotationMatrix.PitchYawRoll(rotation*kAetherRadian);
	Vector3 position = m_cameraOffset._translation;
	position = position.TransformCoordNormal(rotationMatrix);

	view.property._translation = lookAtPosition + position;
	view.property._rotation = rotation + m_cameraOffset._rotation;

}