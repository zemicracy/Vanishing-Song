#include "ActionBoard.h"

#include"ActionShortAttack.h"
#include "ActionLongAttack.h"
#include "ActionRightStep.h"
#include "ActionLeftStep.h"
#include "ActionShield.h"
#include "ActionStrongShield.h"
#include"ActionExAttack.h"
#include"ActionExBuff.h"
#include"ActionExShield.h"
#include"ActionExHeel.h"
#include"ActionNull.h"


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

void ActionBoard::mInitialize(GameManager::eSkillType type){
	WorldReader reader;
	bool result = false;
	SecureZeroMemory(&m_movingAnimation, sizeof(m_movingAnimation[0])*3);
	SecureZeroMemory(&m_frameCnt, sizeof(m_frameCnt[0])*3);
	SecureZeroMemory(&m_changedSuccess, sizeof(m_changedSuccess[0]) * 3);


	reader.Load("data/actionBoard.aether");
	for ( auto itr : reader.GetInputWorldInfo()._object ){
		if (itr->_name == "fore1"){
			gInitalizer<ActionLeftStep>(m_actionCommand[0], itr->_transform, itr->_color,m_kForeground);
			
			continue;
		}
		if (itr->_name == "fore2"){
			gInitalizer<ActionShortAttack>(m_actionCommand[2], itr->_transform, itr->_color, m_kForeground);
			continue;
		}
		if (itr->_name == "fore3"){
			gInitalizer<ActionShield>(m_actionCommand[4], itr->_transform, itr->_color, m_kForeground);
			continue;
		}
		if (itr->_name == "fore4"){
			switch (type)
			{
			case GameManager::eSkillType::eExAttack:
				gInitalizer<ActionExAttack>(m_actionCommand[6], itr->_transform, itr->_color, m_kForeground);
				break;
			case GameManager::eSkillType::eExShield:
				gInitalizer<ActionExShield>(m_actionCommand[6], itr->_transform, itr->_color, m_kForeground);
				break;
			case GameManager::eSkillType::eExHeel:
				gInitalizer<ActionExHeel>(m_actionCommand[6], itr->_transform, itr->_color, m_kForeground);
				break;
			case GameManager::eSkillType::eExBuff:
				gInitalizer<ActionExBuff>(m_actionCommand[6], itr->_transform, itr->_color, m_kForeground);
				break;
			case GameManager::eSkillType::eNull:
				gInitalizer<ActionNull>(m_actionCommand[6], itr->_transform, itr->_color, m_kForeground);
				break;
			}
			continue;
		}

		if (itr->_name == "back1"){
			gInitalizer<ActionRightStep>(m_actionCommand[1], itr->_transform, itr->_color, m_kBackground);
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
	if (GameController::GetMouse().IsRightButtonTrigger()){
		for (int i = 0; i < m_actionCommand.size()-1; i += 2){
			if (mIntersect(m_actionCommand[i])){
				if (!m_movingAnimation[int(i/2)]){
					m_movingAnimation[int(i/ 2)] = true;
				}
			}
		}
	}
	for (int i = 0; i < int(m_actionCommand.size() / 2); ++i){
		if (m_movingAnimation[i]){
			m_frameCnt[i] += 10 * timeScale;
			mChangeAnimation(timeScale, i);
		}
		else m_frameCnt[i] = 0;
	}

}

void ActionBoard::mChangeGround(int target){
	auto tmp = m_actionCommand[target];
	m_actionCommand[target] = m_actionCommand[target + 1];
	m_actionCommand[target + 1] = tmp;

}

std::shared_ptr<ActionCommand> ActionBoard::mSelectType(){
	if (GameController::GetMouse().IsLeftButtonTrigger()){
		for (auto itr : m_actionCommand){
			if (mIntersect(itr)){
				return itr;
			}
		}
	}
	if (GameController::GetKey().KeyDownTrigger('1')){
		return m_actionCommand[0];
	}
	else if (GameController::GetKey().KeyDownTrigger('2')){
		return m_actionCommand[2];
	}	else if (GameController::GetKey().KeyDownTrigger('3')){
		return m_actionCommand[4];
	}	else if (GameController::GetKey().KeyDownTrigger('4')){
		return m_actionCommand[6];
	}
	return nullptr;
}

void ActionBoard::mChangeAnimation(float timeScale, int target){
	float moveVol = 7.6;
	if (m_frameCnt[target] >= 140){
		m_frameCnt[target] = 140;
	}

	if (m_frameCnt[target] > 90 && !m_changedSuccess[target]){
		mChangeGround(target*2);
		m_actionCommand[target * 2]->mGetProperty()._transform._translation._z = m_kForeground;
		m_actionCommand[target * 2 + 1]->mGetProperty()._transform._translation._z = m_kBackground;
		m_changedSuccess[target] = true;
	}

	m_actionCommand[target * 2]->mGetProperty()._transform._translation._x += moveVol * sin(m_frameCnt[target] * kAetherRadian) * timeScale;
	m_actionCommand[target * 2]->mGetProperty()._transform._translation._y += moveVol * sin(m_frameCnt[target] * kAetherRadian) * timeScale;

	m_actionCommand[target * 2 + 1]->mGetProperty()._transform._translation._x -= moveVol * sin(m_frameCnt[target] * kAetherRadian) * timeScale;
	m_actionCommand[target * 2 + 1]->mGetProperty()._transform._translation._y -= moveVol * sin(m_frameCnt[target] * kAetherRadian) * timeScale;
	if (m_frameCnt[target] >= 140){
		m_frameCnt[target] = 0;
		m_movingAnimation[target] = false;
		m_changedSuccess[target] = false;
	}
}

bool ActionBoard::mIntersect(std::shared_ptr<ActionCommand> itr){
			DirectXEntity direct3d;
			Vector3 position = itr->mGetProperty()._transform._translation;
			//
			if (position._z >= 0.5)	return false;

			Vector3 size = itr->mGetProperty()._transform._scale;
			Vector2 mousePosition = GameController::GetMouse().GetMousePosition();
			Vector2 screenSize = aetherFunction::GetWindowSize(direct3d.GetWindowHandle(L"Game"));
			//�E�B���h�E�T�C�Y����
			mousePosition._x = (mousePosition._x / (screenSize._x - GetSystemMetrics(SM_CXDLGFRAME) * 2))* screenSize._x;
			mousePosition._y = (mousePosition._y / (screenSize._y - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CXDLGFRAME)))* screenSize._y;

			//�������Ă��炻���Ԃ�
			if (mousePosition._x > position._x && mousePosition._y > position._y &&
				mousePosition._x < position._x + size._x && mousePosition._y < position._y + size._y){
				return true;
			}
			return false;
}

