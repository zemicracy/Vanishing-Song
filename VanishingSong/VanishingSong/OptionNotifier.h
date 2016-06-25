#pragma once
#include<ViewCamera.h>
#include<Rectangle3D.h>
#include<unordered_map>
#include"OrderList.h"
class OptionNotifier
{
public:
	OptionNotifier();
	~OptionNotifier();


	void mInitialize(aetherClass::ViewCamera*camera, aetherClass::Vector3& pos);
	void mRender(aetherClass::ShaderBase * shader);


	void mSetActive(bool flg);
	void mChangeTexture(OrderList::eAppendOption state);
private:
	void mFinalize();

	bool m_active;

	std::shared_ptr<aetherClass::ModelBase>m_pRect;
	std::unordered_map<OrderList::eAppendOption, std::shared_ptr<aetherClass::Texture>>m_textureList;


};

