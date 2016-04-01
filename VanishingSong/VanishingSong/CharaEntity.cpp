#include "CharaEntity.h"
#include "Debug.h"

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

void CharaEntity::mCreateRelationship(std::shared_ptr<Gear> parentGear, std::shared_ptr<Gear> child){
	parentGear->_pChildren.push_back(child);
	/*	パーツを連結する処理	*/
	child->_pParent = parentGear;
}

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