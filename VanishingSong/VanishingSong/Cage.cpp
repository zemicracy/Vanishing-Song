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

// 管理しやすいように登録
void Cage::mRegisterGearHash(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>& hash, std::shared_ptr<GearFrame>&gearFrame){
	m_charaEntity.mRegisterParts(hash, Gear::eType::eBody, gearFrame->m_pBody);
	m_charaEntity.mRegisterParts(hash, Gear::eType::eWaist, gearFrame->m_pWaist);

	// 左
	m_charaEntity.mRegisterParts(hash, Gear::eType::eLeftArm, gearFrame->m_pLeftArm);
	m_charaEntity.mRegisterParts(hash, Gear::eType::eLeftFoot, gearFrame->m_pLeftFoot);
	// 右
	m_charaEntity.mRegisterParts(hash, Gear::eType::eRightArm, gearFrame->m_pRightArm);
	m_charaEntity.mRegisterParts(hash, Gear::eType::eRightFoot, gearFrame->m_pRightFoot);
}

//
void Cage::mLoadModel(std::shared_ptr<GearFrame>& gearFrame, std::string modelDataFile){
	WorldReader read;
	bool result = read.Load(modelDataFile.c_str());
	if (!result)
	{
		Debug::mErrorPrint("ファイルの読み込みに失敗しました", __FILE__, __LINE__);
	}

	m_topGear = gearFrame->m_pBody;
	for (auto index : read.GetInputWorldInfo()._object){

		/*	体	*/
		if (index->_name == "Body"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pBody, index);
		}

		if (index->_name == "Waist"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pWaist, index);
		}

		/*	左上半身*/
		if (index->_name == "LeftArm"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftArm, index);
		}

		/*	右上半身	*/
		if (index->_name == "RightArm"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightArm, index);
		}

		/*	右足	*/
		if (index->_name == "RightFoot"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pRightFoot, index);
		}

		/*	左足	*/
		if (index->_name == "LeftFoot"){
			m_charaEntity.mSetLoadGearValue(m_topGear, gearFrame->m_pLeftFoot, index);
		}

	}

	read.UnLoad();

	return;
}

//
void Cage::mPrisonerAnimation(){
	// 設定されていない場合何もしない
	Transform animationTransform;

	/*	アニメーション実行処理	*/
	const int allFrame = m_stayAnimation._animationFrame;

	for (auto index : m_stayAnimation._animation)
	{
		// 補間の値を取得
		animationTransform = m_charaEntity.mGetTransformInterpolation(index._start, index._end, allFrame, m_animationCounter);
		// 反転用、体だけ反転させる
		if (index._name == Gear::eType::eBody){
			animationTransform._scale._x = -1;
		}
		// アニメーションの適用
		if (m_gearHash.find(index._name) != m_gearHash.end()){
			m_gearHash[index._name]->_pGear->property._transform = animationTransform;
		}
	}
	/*	カウンターの状態を切り替える	*/
	if (m_animationCounter > allFrame){
		m_changeAnimation = true;
	}
	else if (m_animationCounter < NULL){
		m_changeAnimation = false;
	}

	// カウンターの状態に合わせてフレームカウントの更新
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
		Debug::mErrorPrint("読み込み失敗", __FILE__, __LINE__);
		return;
	}
	animation._animationFrame = allFrame;

	// 登録
	m_stayAnimation = animation;
	return;
}