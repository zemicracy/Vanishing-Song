#ifndef _EQUIPMENT_H
#define _EQUIPMENT_H
/*
	‘•”õŒn‚ÌŠî’êƒNƒ‰ƒX
*/
#include <memory>
#include <FbxModel.h>
#include <Cube.h>
#include <ShaderBase.h>
#include <string>
class Equipment
{
public:
	Equipment();
	virtual ~Equipment() = default;
	bool mCreate(aetherClass::ViewCamera*, std::string);
	void mUpdate(const float);
	void mRender(aetherClass::ShaderBase*);
	std::shared_ptr<aetherClass::Cube> mGetCollider();
	aetherClass::Transform& mGetTransform();
private:
	void mDestroy();
	virtual void mOnUpdate(const float){}
private:
	std::shared_ptr<aetherClass::FbxModel> m_pObject;
	std::shared_ptr<aetherClass::Cube> m_pCollider;
};

#endif