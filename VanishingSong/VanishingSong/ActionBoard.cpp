#include "ActionBoard.h"

#include"ActionShortAttack.h"
#include "ActionLongAttack.h"
#include "ActionRightStep.h"
#include "ActionLeftStep.h"
#include "ActionShield.h"
#include "ActionStrongShield.h"
#include "ActionSkill.h"

#include"Debug.h"
#include"Const.h"

#include<ModelUtility.h>
#include<GameController.h>
#include<WorldReader.h>

using namespace aetherClass;
ActionBoard::ActionBoard()
{
}


ActionBoard::~ActionBoard(){
	mFinalize();
}

void ActionBoard::mFinalize(){
	for (auto itr : m_actionCommand){
		itr.reset();
		itr = nullptr;
	}
}

template<typename T>
void gInitalizer(std::shared_ptr<ActionCommand> &command,Transform transform,Color color,const float kFaze){
	command = std::make_shared<T>();
	command->mCreate();
	command->mGetProperty()._transform = transform;
	command->mGetProperty()._transform._translation._x *= kResolutionFromEditor_x;
	command->mGetProperty()._transform._translation._y *= kResolutionFromEditor_y;
	command->mGetProperty()._transform._translation._z = kFaze;
	command->mGetProperty()._transform._scale._x *= kResolutionFromEditor_x;
	command->mGetProperty()._transform._scale._y *= kResolutionFromEditor_x;

	command->mGetProperty()._color = color;
}

void ActionBoard::mInitialize(){
	WorldReader reader;
	bool result = false;
	reader.Load("data/actionBoard.aether");
	for ( auto itr : reader.GetInputWorldInfo()._object ){
		if (itr->_name == "fore1"){
			gInitalizer<ActionLeftStep>(m_actionCommand[0], itr->_transform, itr->_color,m_kForeground);
			
			continue;
		}
		if (itr->_name == "fore2"){
			gInitalizer<ActionShortAttack>(m_actionCommand[2], itr->_transform, itr->_color, m_kForeground);
			//result = m_actionCommand[2]->LoadAnimation(m_actionCommand[2]->GetAnimationValue(),"data\\PlayerModel2.aether", "data\\PlayerWait.aether");
			
			continue;
		}
		if (itr->_name == "fore3"){
			gInitalizer<ActionShield>(m_actionCommand[4], itr->_transform, itr->_color, m_kForeground);
			continue;
		}
		if (itr->_name == "fore4"){
			gInitalizer<ActionSkill>(m_actionCommand[6], itr->_transform, itr->_color, m_kForeground);
			continue;
		}

		if (itr->_name == "back1"){
			gInitalizer<ActionLeftStep>(m_actionCommand[1], itr->_transform, itr->_color, m_kBackground);
			continue;
		}
		if (itr->_name == "back2"){
			gInitalizer<ActionLongAttack>(m_actionCommand[3], itr->_transform, itr->_color, m_kBackground);
			continue;
		}
		if (itr->_name == "back3"){
			gInitalizer<ActionStrongShield>(m_actionCommand[5], itr->_transform, itr->_color, m_kBackground);
			continue;
		}
	}

	reader.UnLoad();
}


void ActionBoard::mRender(ShaderBase* shader){
	for (int i = m_actionCommand.size() - 1; i >= 0; --i){
		m_actionCommand[i]->mRender(shader);
	}
}
void ActionBoard::mUpdate(float timeScale){
	
}

std::shared_ptr<ActionCommand> ActionBoard::mSelectType(){
	if (GameController::GetMouse().IsLeftButtonTrigger()){
		for (auto itr : m_actionCommand){
			DirectXEntity direct3d;
			Vector3 position = itr->mGetProperty()._transform._translation;
			//
			if (position._z > 0.5)	continue;
			Vector3 size = itr->mGetProperty()._transform._scale;
			Vector2 mousePosition = GameController::GetMouse().GetMousePosition();
			Vector2 screenSize = aetherFunction::GetWindowSize(direct3d.GetWindowHandle(L"Game"));
			//ウィンドウサイズ調整
			mousePosition._x = (mousePosition._x / (screenSize._x - GetSystemMetrics(SM_CXDLGFRAME) * 2))* screenSize._x;
			mousePosition._y = (mousePosition._y / (screenSize._y - GetSystemMetrics(SM_CYCAPTION)- GetSystemMetrics(SM_CXDLGFRAME)))* screenSize._y;

			//当たってたらそれを返す
			if (mousePosition._x > position._x && mousePosition._y > position._y &&
				mousePosition._x < position._x + size._x && mousePosition._y < position._y + size._y){
				return itr;
			}
		}
	}
	return nullptr;
}
