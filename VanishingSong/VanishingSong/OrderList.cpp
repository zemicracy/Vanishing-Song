#include "OrderList.h"


OrderList::OrderList()
{
}


OrderList::~OrderList()
{
}


void OrderList::mUpdate(float){

}
void OrderList::mRender(aetherClass::ShaderBase* shader){
	for (auto itr : m_pSpriteList){
		itr->Render(shader);
	}
}

std::shared_ptr<ActionCommand> OrderList::GetActionCommand(){
	std::shared_ptr<ActionCommand> firstOut = m_orderList[0];
	//Ä¶‚³‚ê‚½‚çH
	if (m_isStart){
		m_orderList.erase(m_orderList.begin());
	}
	return firstOut;
}
int OrderList::GetValume(){
	return 0;
}
