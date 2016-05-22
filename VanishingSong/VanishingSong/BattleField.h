#pragma once
#include<ModelBase.h>
#include<array>
#include"ActionCommand.h"
#include"Const.h"
#include"GameManager.h"
class BattleField
{
public:

	void mInitialize(aetherClass::ViewCamera* view);
	void mUpdate(std::shared_ptr<ActionCommand>command);
	void mRender(aetherClass::ShaderBase *texture, aetherClass::ShaderBase *debug);
	

	BattleField();
	~BattleField();
private:
	void mFinalize();
private:
	
	aetherClass::ViewCamera *m_view;
	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>>m_pTextureList;
	std::unordered_map < eMusical, std::shared_ptr<aetherClass::ModelBase>>m_pLane;

	std::array <std::shared_ptr<aetherClass::ModelBase>, 4>m_command;
	std::array < std::shared_ptr<aetherClass::ModelBase>, 2>m_pGeneral;

	std::array < std::shared_ptr<aetherClass::ModelBase>, 2>m_pDebug;



};

