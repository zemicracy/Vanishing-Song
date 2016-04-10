#include "OrderList.h"
#include<GameController.h>
#include<WorldReader.h>
#include"ResourceManager.h"
#include<Singleton.h>
#include"ActionSound.h"
#include"Const.h"

#include "ActionNull.h"
#include "ActionShortAttack.h"
using namespace aetherClass;
OrderList::OrderList()
{
	m_isStart = false;
	m_volume = 100;
}


OrderList::~OrderList()
{
	mFinalize();
}
void OrderList::mFinalize(){
	m_backImage->Finalize();
	m_backImage.reset();

	for (auto itr : m_spriteList){
		itr->Finalize();
		itr.reset();
	}
	m_orderList.clear();

	m_listFirst = std::make_shared<ActionNull>();
}

void Initializer(std::shared_ptr<SpriteBase> &sprite,Transform transform, Color color){
	sprite = std::make_shared<aetherClass::Rectangle2D>();
	sprite->Initialize();
	sprite->property._transform = transform;
	sprite->property._transform._translation._x *= kResolutionFromEditor_x;
	sprite->property._transform._translation._y *= kResolutionFromEditor_y;
	sprite->property._transform._translation._x -= 10;

	sprite->property._color = color;


}

void OrderList::mInitialize(){
	WorldReader reader;

	reader.Load("data/orderList.aether");

	for (auto itr : reader.GetInputWorldInfo()._object){
		if (itr->_name == "base"){
			Initializer(m_backImage, itr->_transform, itr->_color);
			m_backImage->property._transform._scale._y *= kResolutionFromEditor_y;
			m_backImage->property._transform._translation._x += 10;

		}
		if (itr->_name == "list1"){
			Initializer(m_spriteList[0], itr->_transform, itr->_color);
		}
		if (itr->_name == "list2"){
			Initializer(m_spriteList[1], itr->_transform, itr->_color);
		}
		if (itr->_name == "list3"){
			Initializer(m_spriteList[2], itr->_transform, itr->_color);
		}
		if (itr->_name == "list4"){
			Initializer(m_spriteList[3], itr->_transform, itr->_color);
		}
		if (itr->_name == "list5"){
			Initializer(m_spriteList[4], itr->_transform, itr->_color);
		}
	}
	reader.UnLoad();

	m_listFirst = std::make_shared<ActionNull>();
}

void OrderList::mUpdate(float){
	if (m_orderList.size() == 0){
		return;
	}

	auto sound = Singleton<ResourceManager>::GetInstance().GetActionSound(m_orderList[0]->mGetType());
	if(GameController::GetKey().KeyDownTrigger(VK_SPACE)){
		m_isStart = !m_isStart;
		m_backImage->property._color._red = 1 - m_backImage->property._color._red;
		sound->mPlaySoundAction(m_volume);
	}
	if (m_isStart){

		m_listFirst = m_orderList[0];

		if (sound->mIsPlayEnd() && m_orderList.size() != 0){
			m_orderList.erase(m_orderList.begin());
			if (m_orderList.size() == 0){
				m_backImage->property._color._red = 1 - m_backImage->property._color._red;
				m_isStart = false;
				m_listFirst = std::make_shared<ActionNull>();
				return;
			}
			auto nextSound = Singleton<ResourceManager>::GetInstance().GetActionSound(m_orderList[0]->mGetType());
			nextSound->mPlaySoundAction(m_volume);
		}
	}
	
}
void OrderList::mRender(aetherClass::ShaderBase* shader){
	m_backImage->Render(shader);
	for (int i = 0; i < m_orderList.size(); ++i){
		m_spriteList[i]->property._color = m_orderList[i]->mGetProperty()._color;
		m_spriteList[i]->Render(shader);
	}
}

std::shared_ptr<ActionCommand> OrderList::mGetActionCommand(){
	return m_listFirst;
}
int OrderList::mGetVolume(){
	return 0;
}

void OrderList::mAddOrder(std::shared_ptr<ActionCommand> input){
	if (m_orderList.size() >= m_kMaxOrderSize) return;
	m_orderList.push_back(input);
}

