#include "CharaEntity.h"
#include "Debug.h"
#include "Const.h"
#include "Utility.h"
#include <WorldReader.h>

using namespace aetherClass;

CharaEntity::CharaEntity()
{
}


CharaEntity::~CharaEntity()
{
}

/*
*/
std::shared_ptr<Gear> CharaEntity::mSetUpGear(std::string path, Gear::eType gearType, aetherClass::ViewCamera* view){
	std::shared_ptr<Gear> pGear;
	bool success;

	// 自分のパーツの初期化
	pGear = std::make_shared<Gear>();

	// パーツの本体
	pGear->_pGear = std::make_shared<FbxModel>();

	// 自身の種類の設定
	pGear->_type = gearType;

	// モデルの読み取り
	success = pGear->_pGear->LoadFBX(path, eAxisSystem::eAxisOpenGL);
	if (!success)
	{
		Debug::mErrorPrint("FBXの初期化が正常に行われませんでした。", __FILE__, __FUNCTION__, __LINE__);

		return pGear;
	}

	pGear->_pGear->SetCamera(view);
	// テクスチャの読み込み
	//pGear->_pGear->SetTextureDirectoryName("texture");
	
	return pGear;
}

/*
	親子関係構築用
*/
void CharaEntity::mCreateRelationship(std::shared_ptr<Gear> parentGear, std::shared_ptr<Gear> child){
	// 初期化が正常に終わっていないのなら何もしない
	if (!parentGear || !child||
		!parentGear->_pGear,!child->_pGear)return;

	parentGear->_pChildren.push_back(child);
	
	child->_pParent = parentGear;

	return;
}

/*
	ギアの描画用
	引数できたgearを描画し、そのあとそのgearに子供がいればその描画をする
*/
void CharaEntity::mGearRender(std::shared_ptr<Gear> gear, aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){

	// 初期化が正常に終わっていないのなら何もしない
	if (!gear || !gear->_pGear)return;

	gear->_pGear->Render(colider_shader);

	// 子供がいればその分だけ再帰
	for (auto child : gear->_pChildren){
		mGearRender(child, model_shader, colider_shader);
	}

	return;
}

/*
	ギアの移動用関数
	仕組みはmGearRenderと一緒
*/
void CharaEntity::mGearMove(std::shared_ptr<Gear> gear, const Vector3 move){
	// 初期化が正常に終わっていないのなら何もしない
	if (!gear || !gear->_pGear)return;

	// ギアとそのコライダーを動かす
	gear->_pGear->property._transform._translation += move;

	// 子供がいればその分だけ再帰
	for (auto child : gear->_pChildren){
		mGearMove(child, move);
	}

	return;
}

/*
	ギアを持つオブジェクトの回転用	
	仕組みはmGearRenderと一緒
*/
void CharaEntity::mTopGearRotation(std::shared_ptr<Gear> gear, const Vector3 rotation){
	// 初期化が正常に終わっていないのなら何もしない
	if (!gear || !gear->_pGear)return;
	if (gear->_pParent)
	{

		auto gearRotation = gear->_pParent->_pGear->property._transform._rotation;
		auto gearTranslation = gear->_pParent->_pGear->property._transform._translation;

		
		Matrix4x4 rotationMatrix;
		rotationMatrix.PitchYawRoll(gearRotation*kAetherRadian);
		Vector3 position = gear->_pGear->property._transform._translation;
		position = position.TransformCoordNormal(rotationMatrix);

		gear->_pGear->property._transform._translation = gearTranslation + position;

	}
	
	gear->_pGear->property._transform._rotation += rotation;

	// 子供がいればその分だけ再帰
	for (auto child : gear->_pChildren){
		mTopGearRotation(child, rotation);
	}


	return;
}


//
void CharaEntity::mPartsGearRotation(std::shared_ptr<Gear> gear, const aetherClass::Vector3 rotation){
	// 初期化が正常に終わっていないのなら何もしない
	if (!gear || !gear->_pGear || !gear->_pParent)return;

	auto gearRotation = gear->_pParent->_pGear->property._transform._rotation;
	auto gearTranslation = gear->_pParent->_pGear->property._transform._translation;

	gear->_pGear->property._transform._rotation = gearRotation + gear->_parentDifference._rotation;

	Matrix4x4 rotationMatrix;
	rotationMatrix.PitchYawRoll(gearRotation*kAetherRadian);
	Vector3 position = gear->_parentDifference._translation;
	position = position.TransformCoordNormal(rotationMatrix);

	gear->_pGear->property._transform._translation = gearTranslation + position;

	gear->_parentDifference._rotation += rotation;

	// 子供がいればその分だけ再帰
	for (auto child : gear->_pChildren){
		mPartsGearRotation(child, rotation);
	}

	return;
}


Transform CharaEntity::mGetTransformInterpolation(Transform first, Transform last, const int allFrame, const int nowFrame){
	Transform output;

	// 移動
	output._translation = gInterpolation<Vector3>(first._translation, last._translation, allFrame, nowFrame);

	// 回転
	output._rotation = gInterpolation<Vector3>(first._rotation, last._rotation, allFrame, nowFrame);

	// 拡大縮小
	output._scale = gInterpolation<Vector3>(first._scale, last._scale, allFrame, nowFrame);

	return output;
}


/*
ギアを持つオブジェクトの回転用
体全体を均一に回転する
仕組みはmGearRenderと一緒
キーフレームアニメーション用
*/
void CharaEntity::mBodyGearKeyframeRotation(std::shared_ptr<Gear> gear, const aetherClass::Vector3 rotation){
	// 初期化が正常に終わっていないのなら何もしない
	if (!gear || !gear->_pGear)return;

	if (gear->_pParent)
	{

		auto gearRotation = gear->_pParent->_pGear->property._transform._rotation;
		auto gearTranslation = gear->_pParent->_pGear->property._transform._translation;

		gear->_pGear->property._transform._rotation = gearRotation + gear->_parentDifference._rotation;

		Matrix4x4 rotationMatrix;
		rotationMatrix.PitchYawRoll(gearRotation*kAetherRadian);
		Vector3 position = gear->_parentDifference._translation;
		position = position.TransformCoordNormal(rotationMatrix);

		gear->_pGear->property._transform._translation = gearTranslation + position;
	}
	gear->_pGear->property._transform._rotation = rotation;

	// 子供がいればその分だけ再帰
	for (auto child : gear->_pChildren){
		mBodyGearKeyframeRotation(child, rotation);
	}


}
/*
ギアを持つオブジェクトの回転用
部分部分を回転できる
仕組みはmGearRenderと一緒
キーフレームアニメーション用
*/
void CharaEntity::mPartsGearKeyframeRotation(std::shared_ptr<Gear> gear, const aetherClass::Vector3 rotation){
	// 初期化が正常に終わっていないのなら何もしない
	if (!gear || !gear->_pGear || !gear->_pParent)return;

	auto gearRotation = gear->_pParent->_pGear->property._transform._rotation;
	auto gearTranslation = gear->_pParent->_pGear->property._transform._translation;

	gear->_pGear->property._transform._rotation = gearRotation + gear->_parentDifference._rotation;

	Matrix4x4 rotationMatrix;
	rotationMatrix.PitchYawRoll(gearRotation*kAetherRadian);
	Vector3 position = gear->_parentDifference._translation;
	position = position.TransformCoordNormal(rotationMatrix);

	gear->_pGear->property._transform._translation = gearTranslation + position;

	gear->_parentDifference._rotation = rotation;

	// 子供がいればその分だけ再帰
	for (auto child : gear->_pChildren){
		mPartsGearKeyframeRotation(child, rotation);
	}
}


/*
ギアを持つオブジェクトの移動用
仕組みはmGearRenderと一緒
*/
void CharaEntity::mGearKeyframeTranslation(std::shared_ptr<Gear> gear, aetherClass::Vector3 move){
	// 初期化が正常に終わっていないのなら何もしない
	if (!gear || !gear->_pGear)return;

	// ギアとそのコライダーを動かす
	gear->_pGear->property._transform._translation = move+gear->_topDifference._translation;
	gear->_pGear->property._transform._translation = move + gear->_topDifference._translation;

	// 子供がいればその分だけ再帰
	for (auto child : gear->_pChildren){
		mGearKeyframeTranslation(child, move);
	}

}

Gear::eType CharaEntity::mSetPartsValue(std::string partsName, Transform* input, Transform value){
	/*	体	*/
	if (partsName == "Body"){

		*input = value;
		return Gear::eType::eBody;
	}

	if (partsName == "Waist"){
		*input = value;
		return Gear::eType::eWaist;
	}

	/*	左上半身*/
	if (partsName == "LeftUpperArm"){
		*input = value;
		return Gear::eType::eLeftUpperArm;
	}

	if (partsName == "LeftLowerArm"){
		*input = value;
		return Gear::eType::eLeftLowerArm;
	}

	if (partsName == "LeftHand"){
		*input = value;

		return Gear::eType::eLeftHand;
	}

	/*	右上半身	*/
	if (partsName == "RightUpperArm"){
		*input = value;
		return Gear::eType::eRightUpperArm;
	}

	if (partsName == "RightLowerArm"){
		*input = value;
		return Gear::eType::eRightLowerArm;
	}

	if (partsName == "RightHand"){
		*input = value;
		return Gear::eType::eRightHand;
	}

	/*	右足	*/
	if (partsName == "RightUpperLeg"){
		*input = value;
		return Gear::eType::eRightUpperLeg;
	}

	if (partsName == "RightLowerLeg"){
		*input = value;
		return Gear::eType::eRightLowerLeg;
	}

	/*	左足	*/
	if (partsName == "LeftUpperLeg"){
		*input = value;
		return Gear::eType::eLeftUpperLeg;
	}

	if (partsName == "LeftLowerLeg"){
		*input = value;
		return Gear::eType::eLeftLowerLeg;
	}

	return Gear::eType::eNull;
}

/*
*/
bool CharaEntity::mLoadAnimation(std::vector<Animation>&animationVector,std::string startState, std::string endState){
	WorldReader read;
	bool result;
	result = read.Load(startState);
	if (!result)
	{
		return false;
	}
	Animation animation;
	for (auto index : read.GetInputWorldInfo()._object)
	{
		animation._name = mSetPartsValue(index->_name, &animation._start, index->_transform);
		animationVector.push_back(animation);
	}
	read.UnLoad();

	result = read.Load(endState);
	if (!result)
	{
		return false;
	}

	for (auto index : read.GetInputWorldInfo()._object)
	{
		for (auto& endIndex : animationVector)
		{
			Gear::eType type;
			type = mSetPartsValue(index->_name, &animation._end, index->_transform);

			if (endIndex._name == type)
			{
				endIndex._end = animation._end;
			}
		}
	}

	read.UnLoad();

	return true;
}