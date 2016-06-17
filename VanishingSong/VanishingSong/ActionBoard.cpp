#include "ActionBoard.h"

#include"ActionNull.h"
#include"ActionBlue.h"
#include"ActionGreen.h"
#include"ActionYellow.h"
#include"ActionRed.h"
#include"ActionAdlib.h"

#include"ResourceManager.h"

#include"Debug.h"
#include"Const.h"

#include<ModelUtility.h>
#include<GameController.h>
#include"ActionMiss.h"

using namespace aetherClass;
ActionBoard::ActionBoard()
{
}


ActionBoard::~ActionBoard(){
	mFinalize();
}

void ActionBoard::mFinalize(){
	m_actionList.clear();
	m_playerOrder.clear();
}

template <class T>
void gInitializer(std::unordered_map<eMusical,ActionBoard::ActionCommandType>& tree,Color color,std::string str){
	
	std::shared_ptr<ActionCommand> command = std::make_shared<T>();
	command->mCreate();

	auto tex = ResourceManager::mGetInstance().GetTexture(str);

	command->mSetTexture(tex.get());

	ActionBoard::ActionCommandType act;
	act._command = command;
	act._texture = tex;
	
	tree.insert(std::make_pair(command->mGetType(),act));
}

bool ActionBoard::mInitialize(bool flg){
	bool result = false;
	m_openKey = flg;
	gInitializer<ActionGreen>(m_actionList, Color(0, 1, 0, 1),"ActionGreen");
	gInitializer<ActionBlue>(m_actionList, Color(0, 0, 1, 1), "ActionBlue");
	gInitializer<ActionRed>(m_actionList, Color(1, 0, 0, 1), "ActionRed");
	gInitializer<ActionYellow>(m_actionList, Color(1, 1, 0, 1), "ActionYellow");
	gInitializer<ActionNull>(m_actionList, Color(1, 0, 1, 1), "ActionNull");
	gInitializer<ActionMiss>(m_actionList, Color(1, 0, 1, 1), "ActionMiss");
	gInitializer<ActionAdlib>(m_actionList, Color(1, 0, 1, 1), "ActionAdlib");

	for (auto& index : GameManager::mGetInstance().mGetUsePlayer()){
		m_playerOrder[index.first] = true;
	}
	
	return true;
}


//void ActionBoard::mRender(ShaderBase* shader){
//	return;
//}
//void ActionBoard::mUpdate(float timeScale){
//	return;
//}


std::shared_ptr<ActionCommand> ActionBoard::mSelectType(){
	if ((GameController::GetJoypad().ButtonPress(eJoyButton::eX) || GameController::GetKey().KeyDownTrigger('1'))
		&& (m_playerOrder[eMusical::eBlue] || m_openKey)){
			return m_actionList[eMusical::eBlue]._command;
		}
		else if ((GameController::GetJoypad().ButtonPress(eJoyButton::eA) || GameController::GetKey().KeyDownTrigger('2')) 
			&& (m_playerOrder[eMusical::eGreen] || m_openKey)){
			return m_actionList[eMusical::eGreen]._command;
		}
		else if ((GameController::GetJoypad().ButtonPress(eJoyButton::eB) || GameController::GetKey().KeyDownTrigger('3'))
			&& (m_playerOrder[eMusical::eRed] || m_openKey)){
			return m_actionList[eMusical::eRed]._command;
		}
		else if ((GameController::GetJoypad().ButtonPress(eJoyButton::eY) || GameController::GetKey().KeyDownTrigger('4'))
			&& (m_playerOrder[eMusical::eYellow] || m_openKey)){
			return m_actionList[eMusical::eYellow]._command;
		}
	return nullptr;
}


std::shared_ptr<ActionCommand> ActionBoard::mGetCommand(eMusical type){
	return m_actionList[type]._command;
}
std::shared_ptr<Texture> ActionBoard::mGetCommandTexture(eMusical type){
	return m_actionList[type]._texture;
}
