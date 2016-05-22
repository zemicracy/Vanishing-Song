#include "ActionBoard.h"

#include"ActionNull.h"
#include"ActionBlue.h"
#include"ActionGreen.h"
#include"ActionYellow.h"
#include"ActionRed.h"


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
}

template <class T>
void mInitializer(std::unordered_map<eMusical,ActionBoard::ActionCommandType>& tree,Color color, std::string path){
	
	std::shared_ptr<ActionCommand> command = std::make_shared<T>();
	command->mCreate();                                                                                                                                                                                                                                                                                                          

	auto tex = std::make_shared<Texture>();
	tex->Load(path);
	command->mSetTexture(tex.get());

	ActionBoard::ActionCommandType act;
	act._command = command;
	act._texture = tex;
	
	tree.insert(std::make_pair(command->mGetType(),act));
}

bool ActionBoard::mInitialize(){
	bool result = false;
	
	std::string folder = "Texture\\ActionCommand\\";

	mInitializer<ActionGreen>(m_actionList,Color(0, 1, 0, 1), folder + "Green.png");
	mInitializer<ActionBlue>(m_actionList, Color(0, 0, 1, 1), folder + "Blue.png");
	mInitializer<ActionRed>(m_actionList,Color(1, 0, 0, 1), folder + "Red.png");
	mInitializer<ActionYellow>(m_actionList,Color(1, 1, 0, 1), folder + "Yellow.png");
	mInitializer<ActionNull>(m_actionList, Color(1, 0, 1, 1), folder + "null.png");
	mInitializer<ActionMiss>(m_actionList, Color(1, 0, 1, 1), folder + "miss.png");

	return true;
}


//void ActionBoard::mRender(ShaderBase* shader){
//	return;
//}
//void ActionBoard::mUpdate(float timeScale){
//	return;
//}


std::shared_ptr<ActionCommand> ActionBoard::mSelectType(){
	if (GameController::GetKey().KeyDownTrigger('1')){
		return m_actionList[eMusical::eBlue]._command;
	}
	else if (GameController::GetKey().KeyDownTrigger('2')){
		return m_actionList[eMusical::eGreen]._command;
	}	else if (GameController::GetKey().KeyDownTrigger('3')){
		return m_actionList[eMusical::eRed]._command;
	}	else if (GameController::GetKey().KeyDownTrigger('4')){
		return m_actionList[eMusical::eYellow]._command;
	}

	/*if (GameController::GetJoypad().ButtonPress(eJoyButton::eA)){
		return m_actionList[eMusical::eGreen]._command;

	}
	else if (GameController::GetJoypad().ButtonPress(eJoyButton::eB)){
		return m_actionList[eMusical::eRed]._command;

	}
	else if (GameController::GetJoypad().ButtonPress(eJoyButton::eX)){
		return m_actionList[eMusical::eBlue]._command;

	}
	else if (GameController::GetJoypad().ButtonPress(eJoyButton::eY)){
		return m_actionList[eMusical::eYellow]._command;

	}*/

	return nullptr;
}


std::shared_ptr<ActionCommand> ActionBoard::mGetCommand(eMusical type){
	return m_actionList[type]._command;
}
std::shared_ptr<Texture> ActionBoard::mGetCommandTexture(eMusical type){
	return m_actionList[type]._texture;
}
