#pragma once


#include"Rectangle2D.h"
#include<memory>
class EffectGenerator2D
{
public:
	EffectGenerator2D();
	~EffectGenerator2D();

	//Texture load method
	void mLoadEffect(int maxData, std::string EffectName, std::string folderpath);
	//Effect Starter
	void mPlay(std::string Effectname, std::string instanceName, aetherClass::Transform transform);

	//absolute need.
	bool mInitialize();
	void mRender(aetherClass::ShaderBase* shader);
	void mUpdate(float timescale);

	//accessor Method
	aetherClass::Transform& mGetTransform(std::string key);

private:
	void mFinalize();

	std::shared_ptr<aetherClass::SpriteBase>m_pRect;

	//All Key is same Key
	std::unordered_map<std::string, std::vector<std::shared_ptr<aetherClass::Texture>>>m_textureList;

	//effect management struct
	struct EffectInfo{
		std::vector<std::shared_ptr<aetherClass::Texture>>::iterator _iterator;
		aetherClass::Transform _transform;
		std::string _Effectname;
	};
	std::unordered_map<std::string, EffectInfo>m_effectList;

};

