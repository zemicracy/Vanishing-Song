#include "ActionBoard.h"

#include"ActionShortAttack.h"
#include"Debug.h"

ActionBoard::ActionBoard()
{
}


ActionBoard::~ActionBoard()
{
}

void ActionBoard::mInitialize(){
	for (int i = 0; i < m_actionCommand.size(); i++){
		m_actionCommand[i] = std::make_shared<ActionShortAttack>();
		m_actionCommand[i]->mCreate();
		m_actionCommand[i]->mGetProperty()._color = aetherClass::Color(1, 1, 1, 1);
		m_actionCommand[i]->mGetProperty()._transform._scale = aetherClass::Vector3(100, 100, 1);
		m_actionCommand[i]->mGetProperty()._transform._translation = aetherClass::Vector3(240+120*i, 500, 0);
	}
}


void ActionBoard::mRender(aetherClass::ShaderBase* shader){
	for (auto itr : m_actionCommand){
		Debug::mPrint("Call"+std::to_string(__LINE__));
		itr->mRender(shader);
	}
	Debug::mPrint("");

}
void ActionBoard::mUpdate(float timeScale){

}
std::shared_ptr<ActionCommand> ActionBoard::SelectType(){
	return nullptr;
}
