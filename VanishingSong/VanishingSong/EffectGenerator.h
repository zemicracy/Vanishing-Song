#pragma once

/*
	Attention
	This Effect doesn't support multiple same Effect.
	As much as possible don't use same Effect.
*/

#include"Rectangle3D.h"
#include"Cube.h"
class EffectGenerator
{
public:
	EffectGenerator();
	~EffectGenerator();

	//Texture load method
	void mLoadEffect(int maxData, std::string EffectName, std::string folderpath);
	//Effect Starter
	void mPlay(std::string Effectname,std::string instanceName, aetherClass::Transform transform, aetherClass::Vector3 flgBillboard = aetherClass::Vector3(1.0f, 1.0f, 1.0f));

	//absolute need.
	bool mInitialize(aetherClass::ViewCamera*);
	void mRender(aetherClass::ShaderBase* shader);
	void mUpdate(float timescale);
	
	//accessor Method
	aetherClass::Transform& mGetTransform(std::string key);

private:
	void mFinalize();
	
	std::shared_ptr<aetherClass::ModelBase>m_pRect;
	aetherClass::ViewCamera* m_camera;

	//All Key is same Key
	std::unordered_map<std::string, std::vector<std::shared_ptr<aetherClass::Texture>>>m_textureList;

	//effect management struct
	struct EffectInfo{
		std::vector<std::shared_ptr<aetherClass::Texture>>::iterator _iterator;
		aetherClass::Transform _transform;
		aetherClass::Vector3 _flgBillboard;
		std::string _Effectname;
	};
	std::unordered_map<std::string,EffectInfo>m_effectList;
	ID3D11DepthStencilState *m_depthStencilState;

};

