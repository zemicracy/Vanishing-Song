#pragma once
#include"ActionCommand.h"
#include<array>
class OrderList
{
public:
	OrderList();
	~OrderList();

	void mUpdate(float);
	void mRender(aetherClass::ShaderBase*);
	std::shared_ptr<ActionCommand> GetActionCommand();
	int GetValume();
	void AddOrder();

private:
	std::vector<std::shared_ptr<ActionCommand>>m_orderList;
	std::array<std::shared_ptr<aetherClass::SpriteBase>,5>m_pSpriteList;
	bool m_isStart;
//	none m_ActionSound;
};

