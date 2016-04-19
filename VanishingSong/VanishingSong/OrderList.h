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
	void mAddOrder(std::shared_ptr<ActionCommand>);
	void mInitialize();

	std::shared_ptr<ActionCommand> mGetActionCommand();
	int mGetVolume();
	bool mIsStart();

private:
	void OrderList::mFinalize();

private:
	void mListPlay();
	void mListStop();

private:
	std::vector<std::shared_ptr<ActionCommand>>m_orderList;
	std::shared_ptr<ActionCommand>m_listFirst;

	std::array<std::shared_ptr<aetherClass::SpriteBase>,5>m_spriteList;
	std::shared_ptr<aetherClass::SpriteBase>m_backImage;
	bool m_isStart;
	float m_volume;
	const char m_kMaxOrderSize = 5;
};

