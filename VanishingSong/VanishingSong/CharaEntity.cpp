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
std::shared_ptr<Gear> CharaEntity::mSetUpGear(std::string path, Gear::eType gearType,  std::string directry){
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

	// テクスチャの読み込み
	pGear->_pGear->SetTextureDirectoryName(directry);
	return pGear;
}

// カメラセット用
void CharaEntity::SetCamera(std::shared_ptr<Gear> gear, aetherClass::ViewCamera* camera){
	if (!gear)return;

	gear->_pGear->SetCamera(camera);

	for (auto child : gear->_pChildren){
		SetCamera(child, camera);
	}
}

/*
	親子関係構築用
*/
void CharaEntity::mCreateRelationship(std::shared_ptr<Gear> parentGear, std::shared_ptr<Gear> child){
	// 初期化が正常に終わっていないのなら何もしない
	if (!parentGear || !child)return;

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

	gear->_pGear->Render(model_shader);

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
void CharaEntity::mGearMove(std::shared_ptr<Gear> gear,Vector3 move, std::string type){
	// 初期化が正常に終わっていないのなら何もしない
	if (!gear || !gear->_pGear)return;

	if (type == "+="){
		// ギアとそのコライダーを動かす
		gear->_pGear->property._transform._translation += move;

	}
	else if (type == "="){
		if (!gear->_pParent){
			gear->_pGear->property._transform._translation = move;
		}
		else{
			gear->_pGear->property._transform._translation = gear->_pParent->_pGear->property._transform._translation +gear->_parentDifference._translation;

		}
	}
	else{
		gear->_pGear->property._transform._translation += move;

	}
	
	// 子供がいればその分だけ再帰
	for (auto child : gear->_pChildren){
		mGearMove(child, move,type);
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
	公転	
*/
void CharaEntity::mGearRotation(std::shared_ptr<Gear> top, std::shared_ptr<Gear> gear, aetherClass::Vector3 rotation, std::string type){
	
	if (!gear || !gear->_pGear)return;
	if (type == "+="){
		gear->_pGear->property._transform._rotation += rotation;
	}
	else if (type == "="){
		gear->_pGear->property._transform._rotation = rotation;
	}

	if (gear->_pParent)
	{
		// 最上位との差を更新
		gear->_topDifference._translation = gear->_pGear->property._transform._translation -top->_pGear->property._transform._translation;
		gear->_topDifference._rotation = gear->_pGear->property._transform._rotation - top->_pGear->property._transform._rotation;

		auto gearRotation = top->_pGear->property._transform._rotation;
		auto gearTranslation = top->_pGear->property._transform._translation;

		Matrix4x4 rotationMatrix;
		Vector3 rotationY = Vector3(0, gearRotation._y, 0);
		rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);

		Vector3 position = gear->_topDifference._translation;
		position = position.TransformCoordNormal(rotationMatrix);
		
		// 公転位置の決定
		gear->_pGear->property._transform._translation = gearTranslation + position;
	}


	
	// 子供がいればその分だけ再帰
	for (auto child : gear->_pChildren){
		mGearRotation(top,child, rotation);
	}

}

/*
指定パーツのみ回転
*/
void CharaEntity::mGearPartsRotation(std::shared_ptr<Gear> top, std::shared_ptr<Gear> gear, Gear::eType notRotaionType, const Vector3 rotation){
	if (!gear || !gear->_pGear)return;


	gear->_pGear->property._transform._rotation += rotation;

	if (gear->_pParent)
	{
		// 最上位との差を更新
		gear->_topDifference._translation = gear->_pGear->property._transform._translation - top->_pGear->property._transform._translation;
		gear->_topDifference._rotation = gear->_pGear->property._transform._rotation - top->_pGear->property._transform._rotation;

		auto gearRotation = top->_pGear->property._transform._rotation;
		auto gearTranslation = top->_pGear->property._transform._translation;

		Matrix4x4 rotationMatrix;
		Vector3 rotationY = Vector3(0, gearRotation._y, 0);
		rotationMatrix.PitchYawRoll(rotationY*kAetherRadian);

		Vector3 position = gear->_topDifference._translation;
		position = position.TransformCoordNormal(rotationMatrix);

		// 公転位置の決定
		gear->_pGear->property._transform._translation = gearTranslation + position;
	}
	
	for (auto child : gear->_pChildren)
	{
		if (child->_type == notRotaionType || child->_type == Gear::eType::eNull) continue;
		mGearPartsRotation(top,child,notRotaionType, rotation);
	}
	return;
}

/*
	連想配列に登録
*/
void CharaEntity::mRegisterParts(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>& hash, Gear::eType type, std::shared_ptr<Gear>& parts){

	// 登録済みなら何もしない
	if (hash.find(type) != hash.end() || !parts)return;

	// 登録
	hash.insert(std::make_pair(type, parts));
	return;
}


/*
パーツの初期化
*/
void CharaEntity::mSetLoadGearValue(std::shared_ptr<Gear>& top,std::shared_ptr<Gear>& gear, ObjectInfo* info){

	gear->_pGear->property._transform = info->_transform;
	gear->_initialTransform = info->_transform;

	// 最上位との差
	gear->_topDifference._translation = gear->_pGear->property._transform._translation - top->_pGear->property._transform._translation;
	gear->_topDifference._rotation = gear->_pGear->property._transform._rotation - top->_pGear->property._transform._rotation;

	if (gear->_pParent)
	{
		std::shared_ptr<Gear> pParent = gear->_pParent;
		// 親との差
		gear->_parentDifference._translation = gear->_pGear->property._transform._translation - pParent->_pGear->property._transform._translation;
		gear->_parentDifference._rotation = gear->_pGear->property._transform._rotation - pParent->_pGear->property._transform._rotation;
	}

	return;
}
/*
	値の代入と
	文字列からenumに変換
*/
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

	if (partsName == "RightFoot"){
		*input = value;
		return Gear::eType::eRightFoot;
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

	if (partsName == "LeftFoot"){
		*input = value;
		return Gear::eType::eLeftFoot;
	}

	return Gear::eType::eNull;
}


//
void CharaEntity::mFaceToObject(std::shared_ptr<Gear>& top, aetherClass::Vector3 facePosition, std::string type){
	float rad = atan2(facePosition._x - top->_pGear->property._transform._translation._x, facePosition._z - top->_pGear->property._transform._translation._z);
	float rotationY = rad / kAetherPI * 180;
	mGearRotation(top, top, Vector3(0, rotationY, 0),type);
}


/*
キーフレームアニメーションを読み込むよう
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