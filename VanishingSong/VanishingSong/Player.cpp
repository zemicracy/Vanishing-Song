#include "Player.h"
#include "Debug.h"
#include "Utility.h"
#include "Bullet.h"
#include <MathUtility.h>
#include <GameController.h>
#include <WorldReader.h>
#include <GameClock.h>
#include <Windows.h>
using namespace aetherClass;

namespace{
	const int kWaitAnimationFrame = 60;
	const int kMoveAnimationFrame = 5;
	const float kCameraRotationMaxX = 15.0f;
	const float kCameraRotationMinX = -15.0f;
	const float kCameraY = 500.0f;
	const Vector3 kColliderOffset = Vector3(0, -5, 0); 
	const Vector3 kPlayerInitialY = Vector3(0, 26, 0);
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

	
	// �M�A�n�̏������p
	result = mInitializeGearFrame(m_pGearFrame, &m_playerView);
	if (!result)
	{
		Debug::mErrorPrint("�M�A�̏������Ɏ��s", __FILE__, __LINE__);
		return false;
	}

	// �����ʒu�̐ݒ�
	m_playerTransform._translation = kPlayerInitialY;

	// �p�[�c�̏����ʒu
	result = mLoadProperty(m_pGearFrame, "data\\PlayerDefault.aether");
	if (!result)
	{
		Debug::mErrorPrint("Editor����̈ʒu���f�Ɏ��s", __FILE__, __LINE__);
		return false;
	}


	// �R���C�_�[�̏�����
	mSetUpBodyCollider(m_pBodyCollider, m_pTopGear->_pGear->property._transform._translation, kColliderOffset);

	/*	��{�I�ȃA�j���[�V�����̓o�^	*/
	mRegisterAnimation(Player::eState::eMove, kMoveAnimationFrame,"data\\PlayerDefault.aether", "data\\PlayerMove.aether");
	mRegisterAnimation(Player::eState::eWait, kWaitAnimationFrame,"data\\PlayerDefault.aether", "data\\PlayerWait.aether");
	if (kCharaDebug)
	{
		Debug::mPrint("�v���C���[ �������I�����܂���");
	}

	// ����n�̏������p
	//mSetupWeapon<Bullet>(m_pOriginalBullets, "Model\\Weapon\\bullet.fbx");
	mSetupBullet(mGetView());
	m_isHitWall = false;
	return true;
}

/*
�������
*/
void Player::mFinalize(){
	
	if (!m_defaultAnimation.empty()){
		for (auto index : m_defaultAnimation){
			index.second._animation.clear();
		}
		m_defaultAnimation.clear();
	}
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

	if (m_pBodyCollider)
	{
		m_pBodyCollider->Finalize();
		m_pBodyCollider.reset();
		m_pBodyCollider = nullptr;
	}

	if (!m_pBullets.empty()){
		for (auto index : m_pBullets){
			if (!index._bullet)continue;
			index._bullet.reset();
			index._bullet = nullptr;

			index._isRun = false;
			index._number = NULL;
		}
	}

	// �X�e�[�^�X�̃��Z�b�g
	m_status.Reset();

	m_prevCommand = eCommandType::eNull;
	m_prevState = eState::eNull;
	m_cameraRotation = kVector3Zero;
	m_pGearHash.clear();

	return;
}


/*
�v���C���[�̍X�V����
*/
void Player::mUpdate(const float timeScale){
	
	// �L�[�̏������擾
	KeyValues getKeyValues = mReadKey(timeScale);

	// �J�����̏���
	m_cameraRotation += getKeyValues._cameraRotation;

	
	mUpdateView(m_playerView, m_cameraRotation, m_pTopGear->_pGear->property._transform._translation);


	// ��{�I�ȃA�j���[�V�����̍Đ�
	mDefaultAnimation(getKeyValues._state);

	// �ړ��Ɏg���l�̂��擾
	Matrix4x4 rotationMatrix;
	Vector3 rotationY = Vector3(0,m_cameraRotation._y,0);
	rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);

	m_playerTransform._rotation._y += getKeyValues._cameraRotation._y;
	// �ǂɓ������Ă��邩�̔���
	if (m_isHitWall){
		m_playerTransform._translation = m_prevTransform._translation;
	}
	else{
		// �J�����̉�]�s����|�����킹�āA�J�����̌����Ɛi�s��������v������
		Vector3 translation = getKeyValues._transform._translation.TransformCoordNormal(rotationMatrix);
		m_playerTransform._translation += translation;
	}

	m_isHitWall = false;
	// �e�̔���
	for (auto index : m_pBullets){
		if (!index._isRun)continue;
		Vector3 bulletSpeed = Vector3(0, 0, 10);
		Vector3 bulletMove = bulletSpeed.TransformCoordNormal(rotationMatrix);
		index._bullet->mGetTransform()._translation += bulletMove;
	}

	// �ړ�����
	m_charaEntity.mGearMove(m_pTopGear, m_playerTransform._translation);

	// ��]����
	m_charaEntity.mGearRotation(m_pTopGear, m_pTopGear, m_playerTransform._rotation);

	// �R���C�_�[�̍X�V����
	mUpdateBodyCollider(m_pTopGear->_pGear->property._transform);
	return;
}

/*
�L�[��ǂݍ���
�Ԃ�͍��̂Ƃ���transform
*/
Player::KeyValues Player::mReadKey(const float timeScale){

	KeyValues output;

	// ���s�̈ړ�(Z��)
	if (GameController::GetKey().IsKeyDown('W')){
		output._transform._translation._z = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('S')){
		output._transform._translation._z = -(GameClock::GetDeltaTime()*timeScale);
	}

	// ���̈ړ�(X��)
	if (GameController::GetKey().IsKeyDown('D')){
		output._transform._translation._x = GameClock::GetDeltaTime()*timeScale;
	}
	else if (GameController::GetKey().IsKeyDown('A')){
		output._transform._translation._x = -(GameClock::GetDeltaTime()*timeScale);
	}

	// �ړ��������
	if (output._transform._translation == kVector3Zero){
		output._state = eState::eWait;
	}
	else{
		output._state = eState::eMove;
	}


	/*	�J�����̉�]	*/
	Vector2 mousePosition = GameController::GetMouse().GetMousePosition();

	/*	�R�}���h��I�[�_�[���X�g�̉ӏ��ȊO�݂̂ɔ�������*/
	if (mousePosition._y < kCameraY){
		if (GameController::GetMouse().IsRightButtonDown()){
			gLockMouseCursor(m_directXEntity.GetWindowHandle(kWindowName), true);
			Vector2 cameraRotation = GameController::GetMouse().GetMouseMovement();
			cameraRotation /= kAetherPI;
			output._cameraRotation._x += cameraRotation._y;
			output._cameraRotation._y += cameraRotation._x;

		}
	}

	return output;
}

//
void Player::mRender(aetherClass::ShaderBase* modelShader, aetherClass::ShaderBase* colliderShader){

	if (!m_pTopGear)return;
	m_playerView.Render();
	// �S�Ă̐e�͑̂̃p�[�c�Ȃ̂ŁA�K���̂̃p�[�c����n�߂�
	m_charaEntity.mGearRender(m_pTopGear, modelShader, colliderShader);

	for (auto index : m_pBullets){
		if (!index._isRun)continue;
		index._bullet->mRender(modelShader);
	}

	if (kCharaDebug)
	{
		m_pBodyCollider->Render(modelShader);
	}

	return;
}

//
eCommandType Player::mCommand(std::shared_ptr<ActionCommand> command, const float timeScale){

	// ������s���A�N�V�������擾
	m_status._nowCommand = command->mGetType();

	// �O��ƈႦ�Ύ��s����0�ɂ���
	if (m_status._nowCommand != m_prevCommand){
		m_actionCount._commandFrame = kZeroPoint;
		command->mCallCount(0);
	}

	// �Ă΂�Ă��Ȃ����
	if (!command->mIsCall()){
		mWeaponRun(m_status._nowCommand, m_actionCount._commandFrame);
	}

	// �A�N�V�����̎��s
	command->mAction(m_pGearHash, timeScale,m_actionCount._commandFrame);

	if (command->mGetType()!= eCommandType::eNull)
	{
		Debug::mPrint("Run Action :" + std::to_string(m_actionCount._commandFrame) + "���");
	}
	
	m_actionCount._commandFrame += 1;
	const int callCount = command->mCallCount();
	command->mCallCount(callCount + 1);

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

std::shared_ptr<Cube> Player::mGetBodyColldier(){
	return m_pBodyCollider;
}

/*
�M�A�n�̏��������܂Ƃ߂�����

*/
bool Player::mInitializeGearFrame(std::shared_ptr<GearFrame>& gearFrame, aetherClass::ViewCamera* camera){
	
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

	// ���ꂼ��̃p�[�c�Ƃ̐e�q�֌W�\�z
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pWaist);
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pRightUpperArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pBody, gearFrame->m_pLeftUpperArm);

	// �E
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightUpperArm, gearFrame->m_pRightLowerArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightLowerArm, gearFrame->m_pRightHand);
	m_charaEntity.mCreateRelationship(gearFrame->m_pWaist, gearFrame->m_pRightUpperLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pRightUpperLeg, gearFrame->m_pRightLowerLeg);

	// ��
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftUpperArm, gearFrame->m_pLeftLowerArm);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftLowerArm, gearFrame->m_pLeftHand);
	m_charaEntity.mCreateRelationship(gearFrame->m_pWaist, gearFrame->m_pLeftUpperLeg);
	m_charaEntity.mCreateRelationship(gearFrame->m_pLeftUpperLeg, gearFrame->m_pLeftLowerLeg);

	//
	// �A�z�z��ɓo�^
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eBody, m_pGearFrame->m_pBody);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eWaist, m_pGearFrame->m_pWaist);

	// ��
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftHand, m_pGearFrame->m_pLeftHand);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftLowerArm, m_pGearFrame->m_pLeftLowerArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftLowerLeg, m_pGearFrame->m_pLeftLowerLeg);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftUpperArm, m_pGearFrame->m_pLeftUpperArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eLeftUpperLeg, m_pGearFrame->m_pLeftUpperLeg);

	// �E
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightHand, m_pGearFrame->m_pRightHand);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightLowerArm, m_pGearFrame->m_pRightLowerArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightLowerLeg, m_pGearFrame->m_pRightLowerLeg);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightUpperArm, m_pGearFrame->m_pRightUpperArm);
	m_charaEntity.mRegisterParts(m_pGearHash, Gear::eType::eRightUpperLeg, m_pGearFrame->m_pRightUpperLeg);

	return true;
}

/*
	�G�f�B�^�[����ǂݎ��
*/
bool Player::mLoadProperty(std::shared_ptr<GearFrame>& gearFrame, std::string modelDataFile){
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
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pBody, index);
		}

		if (index->_name == "Waist"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pWaist, index);
		}

		/*	���㔼�g*/
		if (index->_name == "LeftUpperArm"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pLeftUpperArm, index);
		}

		if (index->_name == "LeftLowerArm"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pLeftLowerArm, index);
		}

		if (index->_name == "LeftHand"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pLeftHand, index);
		}

		/*	�E�㔼�g	*/
		if (index->_name == "RightUpperArm"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pRightUpperArm, index);
		}

		if (index->_name == "RightLowerArm"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pRightLowerArm, index);
		}

		if (index->_name == "RightHand"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pRightHand, index);
		}

		/*	�E��	*/
		if (index->_name == "RightUpperLeg"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pRightUpperLeg, index);
		}

		if (index->_name == "RightLowerLeg"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pRightLowerLeg, index);
		}

		/*	����	*/
		if (index->_name == "LeftUpperLeg"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pLeftUpperLeg, index);
		}

		if (index->_name == "LeftLowerLeg"){
			m_charaEntity.mSetLoadGearValue(m_pTopGear, gearFrame->m_pLeftLowerLeg, index);
		}

	}
	
	mInitialPlayerView(read.GetInputWorldInfo()._camera);
	read.UnLoad();

	return true;
}

/*
	�J�����̏�����
*/
void Player::mInitialPlayerView(CameraValue input){
	// �J�����̏�����
	m_playerView.property._translation = input._position;
	m_playerView.property._rotation = input._rotation;
	
	// �f�o�b�O�p
	m_playerView.property._translation = Vector3(0, 100, -200);

	// �J�����̃I�t�Z�b�g�̐ݒ�
	m_cameraOffset._translation = m_playerView.property._translation;
	m_cameraOffset._rotation = m_playerView.property._rotation;

	return;
}

/*
	�R���C�_�[�̏������p
*/
void Player::mSetUpBodyCollider(std::shared_ptr<aetherClass::Cube>& collider, Vector3 original, Vector3 offset){
	collider = std::make_shared<Cube>();
	collider->Initialize();
	collider->property._transform._translation = original + offset;
	collider->property._transform._scale = 10;
	collider->property._color = Color(1, 0, 0, 0.5);
	collider->SetCamera(&m_playerView);
	return;
}

/*
	�R���C�_�[�̍X�V�p
*/
void Player::mUpdateBodyCollider(Transform& transform){
	m_pBodyCollider->property._transform._translation = transform._translation + kColliderOffset;
	m_pBodyCollider->property._transform._rotation = transform._rotation;

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
	��{�I�ȃA�j���[�V�����̍Đ�
*/
void Player::mDefaultAnimation(Player::eState& m_state){

	// ���݂̃R�}���h��NULL�ȊO�Ȃ�A�j���[�V�����͍Đ����Ȃ�
	//if (m_status._nowCommand != eCommandType::eNull)return;
	
	// �O��ƈႤ�Ƃ��͍X�V
	if (m_prevState != m_state){
		m_actionCount._defaultFrame = NULL;
		m_prevState = m_state;
	}

	// �ݒ肳��Ă��Ȃ��ꍇ�������Ȃ�
	if (m_defaultAnimation.find(m_state) == m_defaultAnimation.end()) return;

	Transform animationTransform;

	/*	�A�j���[�V�������s����	*/
	const int allFrame = m_defaultAnimation[m_state]._animationFrame;

	for (auto index : m_defaultAnimation[m_state]._animation)
	{
		// ��Ԃ̒l���擾
		animationTransform = m_charaEntity.mGetTransformInterpolation(index._start, index._end,allFrame , m_actionCount._defaultFrame);

		// �A�j���[�V�����̓K�p
		if (m_pGearHash.find(index._name) != m_pGearHash.end()){
			m_pGearHash[index._name]->_pGear->property._transform = animationTransform;
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

void Player::mUpdateView(ViewCamera& view,Vector3& rotation,Vector3 lookAtPosition){

	// �J�����̃��Z�b�g�ꉞ�K�Ƃ���
	CheckCameraRotation(rotation);
	Matrix4x4 rotationMatrix;
	rotationMatrix.PitchYawRoll(rotation*kAetherRadian);
	Vector3 position = m_cameraOffset._translation;
	position = position.TransformCoordNormal(rotationMatrix);

	view.property._translation = lookAtPosition + position;
	view.property._rotation = rotation + m_cameraOffset._rotation;

	return;
}


void Player::CheckCameraRotation(Vector3& rotation){
	// �J�������͈͂̏���̊m�F
	if (rotation._x > kCameraRotationMaxX){
		rotation._x = kCameraRotationMaxX;
	}
	else if (rotation._x < kCameraRotationMinX){
		rotation._x = kCameraRotationMinX;
	}
}

template<class type>
void Player::mSetupWeapon(std::shared_ptr<Equipment>& weapon, std::string model){
	weapon = std::make_shared<type>();
	weapon->mCreate(mGetView(),model);
	return;
}

//
void Player::mSetupBullet(ViewCamera* view){
	for (auto&index : m_pBullets){
		index._bullet = std::make_shared<Bullet>();
		index._bullet->mCreate(view,"Model\\Weapon\\bullet.fbx");
		index._isRun = false;
		index._number = 0;
	}
}

//
void Player::mWeaponRun(eCommandType type, const int callFrame){
	switch (type)
	{
	case eCommandType::eShortDistanceAttack:
		break;
	case eCommandType::eLongDistanceAttack:{
		for (auto& index : m_pBullets){
			if (index._isRun)continue;
			index._bullet->mGetTransform()._translation = m_pTopGear->_pGear->property._transform._translation;
			index._isRun = true;
			break;
		}
	}
		break;
	case eCommandType::eRightStep:
		break;
	case eCommandType::eLeftStep:
		break;
	case eCommandType::eShield:
		break;
	case eCommandType::eStrongShield:
		break;
	case eCommandType::eSkill:
		break;
	case eCommandType::eNull:
		break;
	default:
		break;
	}
}

std::array<Player::BulletPool, kMaxBullet>& Player::mGetBullet(){
	return m_pBullets;
}

// �ǂɓ����������̏���
void Player::OnHitWall(){
	Debug::mPrint("�ǂƓ�������");
	m_prevTransform = m_playerTransform;
	m_isHitWall = true;
	return;
}