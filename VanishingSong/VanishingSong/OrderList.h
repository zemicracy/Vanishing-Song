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

	//AccesserMethod
	void mSetBPM(float);
	int mGetVolume();
	bool mIsJustTiming();
private:
	void mFinalize();

	void mListPlay();
	void mListStop();
	void mException();


private:
	std::vector<std::shared_ptr<ActionCommand>>m_orderList;
	std::shared_ptr<ActionCommand>m_listFirst;

	std::array<std::shared_ptr<aetherClass::SpriteBase>,5>m_pSpriteList;
	std::shared_ptr<aetherClass::SpriteBase>m_pVolumeImage;
	std::shared_ptr<aetherClass::SpriteBase>m_pBackImage;
	aetherClass::Vector3 m_volumeOrigin;
	
	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>>m_pTexture;

	bool m_isStart;
	bool m_isPlayCommand;
	bool m_isJustTiming;
	float m_volume;	//���z���l
	float m_bpm;
	float m_timeRadian;
	const char m_kMaxOrderSize = 5;
	const float m_kMaxVolume = 100;
	
};

