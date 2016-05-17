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
void ActionBoard::mInitializer(Color color, std::string path){
	
	auto command = std::make_shared<T>();
	command->mCreate();
	command->mGetProperty()._color = color;

	auto tex = std::make_shared<Texture>();
	tex->Load(path);
	command->mSetTexture(tex.get());

	ActionCommandType act;
	act._command = command;
	act._texture = tex;

	m_actionList.insert(std::make_pair(command->mGetType(), act));
}

bool ActionBoard::mInitialize(){
	bool result = false;
	
	std::string folder = "Texture\\ActionCommand\\";

	mInitializer<ActionGreen>(Color(0, 1, 0, 1), folder + "Green.png");
	mInitializer<ActionBlue>(Color(0, 0, 1, 1), folder + "Blue.png");
	mInitializer<ActionRed>(Color(1, 0, 0, 1), folder + "Red.png");
	mInitializer<ActionYellow>(Color(1, 1, 0, 1), folder + "Yellow.png");
	mInitializer<ActionNull>(Color(1, 0, 1, 1), folder + "null.png");

}


void ActionBoard::mRender(ShaderBase* shader){
	return;
}
void ActionBoard::mUpdate(float timeScale){
	return;
}


std::shared_ptr<ActionCommand> ActionBoard::mSelectType(){
	if (GameController::GetKey().KeyDownTrigger('1')){
		return m_actionList[eCommandType::eBlue]._command;
	}
	else if (GameController::GetKey().KeyDownTrigger('2')){
		return m_actionList[eCommandType::eGreen]._command;
	}	else if (GameController::GetKey().KeyDownTrigger('3')){
		return m_actionList[eCommandType::eRed]._command;
	}	else if (GameController::GetKey().KeyDownTrigger('4')){
		return m_actionList[eCommandType::eYellow]._command;
	}
	return nullptr;
}


std::shared_ptr<ActionCommand> ActionBoard::mGetCommand(eCommandType type){
	return m_actionList[type]._command;
}

