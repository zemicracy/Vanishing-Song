#pragma once
#include<ModelBase.h>
#include<array>
#include"ActionCommand.h"
#include"Const.h"
#include"GameManager.h"
#include"FbxModel.h"
class BattleField
{
public:

	void mInitialize(aetherClass::ViewCamera* view);
	void mUpdate(std::shared_ptr<ActionCommand>command);
	void mRender(aetherClass::ShaderBase *texture, aetherClass::ShaderBase *debug);
	aetherClass::Vector3 mGetEnemyLane(eMusical type);
	aetherClass::Vector3 mGetPlayerLane(eMusical type);
	aetherClass::ViewCamera* mGetCamera();

	BattleField();
	~BattleField();
private:
	void mFinalize();
private:
	
	aetherClass::ViewCamera *m_view;
	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>>m_pTextureList;
	std::unordered_map < eMusical, std::shared_ptr<aetherClass::ModelBase>>m_pLane;
	std::unordered_map < eMusical, aetherClass::Vector3>m_EnemyLane;
	std::unordered_map < eMusical, aetherClass::Vector3>m_PlayerLane;

	std::array <std::shared_ptr<aetherClass::ModelBase>, 4>m_pCommand;
	std::shared_ptr<aetherClass::ModelBase>m_pSkyBox;
	std::shared_ptr<aetherClass::FbxModel>m_pPlane;

	std::array < std::shared_ptr<aetherClass::ModelBase>, 2>m_pDebug;



};

