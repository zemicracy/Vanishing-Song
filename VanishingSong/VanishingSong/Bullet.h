#ifndef _BULLET_H
#define _BULLET_H
#include <memory>
#include <FbxModel.h>
#include <Cube.h>
#include <ShaderBase.h>
#include <string>
class Bullet
{
public:
	Bullet();
	~Bullet();
	bool mCreate(std::string);
	void mUpdate(const float);
	void mRender(aetherClass::ShaderBase*);
	std::shared_ptr<aetherClass::Cube> mGetCollider();
	aetherClass::Vector3& mGetTranslation();
	aetherClass::Vector3& mGetRotation();
private:
	void mDestroy();
private:
	std::shared_ptr<aetherClass::FbxModel> m_pBullet;
	std::shared_ptr<aetherClass::Cube> m_pCollider;
};

#endif