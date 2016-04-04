#include "CharaEntity.h"
#include "Debug.h"
#include "Const.h"
using namespace aetherClass;

CharaEntity::CharaEntity()
{
}


CharaEntity::~CharaEntity()
{
}

std::shared_ptr<Gear> CharaEntity::mSetUpGear(std::string path, Gear::eType gearType, aetherClass::ViewCamera* view){
	std::shared_ptr<Gear> pGear;
	bool success;

	// 自分のパーツの初期化
	pGear = std::make_shared<Gear>();
	pGear->_pGear = std::make_shared<FbxModel>();
	// 自身の種類の設定
	pGear->_type = gearType;

	// モデルの読み取り
	success = pGear->_pGear->LoadFBX(path, eAxisSystem::eAxisOpenGL);
	if (!success)
	{
		Debug::mErrorPrint("FBXの初期化が正常に行われませんでした。", __FILE__, __FUNCTION__, __LINE__);

		if (kCharaDebug)
		{
			// コライダーの初期化
			pGear->_pColider = std::make_shared<Cube>();
			pGear->_pColider->Initialize();
			// コライダーの色を赤に設定
			pGear->_pColider->property._color = Color(1.0f, 0.0f, 0.0f, 0.3f);
			pGear->_pColider->SetCamera(view);
			return pGear;
		}
		return nullptr;
	}

	pGear->_pGear->SetCamera(view);
	// テクスチャの読み込み
	//pGear->_pGear->SetTextureDirectoryName("texture")

	// コライダーの初期化
	pGear->_pColider = std::make_shared<Cube>();
	pGear->_pColider->Initialize();
	// コライダーの色を赤に設定
	pGear->_pColider->property._color = Color(1.0f, 0.0f, 0.0f, 0.3f);
	pGear->_pColider->SetCamera(view);
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
	/*	パーツを連結する処理	*/
	child->_pParent = parentGear;


	// 改善するべき箇所
	// どうするべきか
	/*Vector3 gearPosition = parentGear->_pGear->property._transform._translation;
	Vector3 coliderPosition = parentGear->_pColider->property._transform._translation;

	child->_pGear->property._transform._position = gearPosition;
	child->_pColider->property._transform._position = coliderPosition;*/

	return;
}

/*
	ギアの描画用
	引数できたgearを描画し、そのあとそのgearに子供がいればその描画をする
*/
void CharaEntity::mGearRender(std::shared_ptr<Gear> gear, aetherClass::ShaderBase* model_shader, aetherClass::ShaderBase* colider_shader){

	// デバッグモードの時はコライダーの表示
	if (kCharaDebug)
	{
		gear->_pColider->Render(colider_shader);
	}

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
	gear->_pColider->property._transform._translation += move;

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
void CharaEntity::mGearRotation(std::shared_ptr<Gear> gear, const Vector3 rotation){
	// 初期化が正常に終わっていないのなら何もしない
	if (!gear || !gear->_pGear)return;

	/*if (gear->_pParent)
	{
		gear->_pGear->property._transform._position = gear->_pParent->_pGear->property._transform._translation;
		gear->_pColider->property._transform._position = gear->_pParent->_pColider->property._transform._translation;
	}*/

	// ギアとそのコライダーを動かす
	gear->_pGear->property._transform._rotation += rotation;
	gear->_pColider->property._transform._rotation += rotation;

	// 子供がいればその分だけ再帰
	for (auto child : gear->_pChildren){
		mGearRotation(child, rotation);
	}

	return;
}