#ifndef _CAGE_H
#define _CAGE_H
#include <ShaderBase.h>
#include <Cube.h>
#include "CharaEntity.h"
#include <array>
#include "MessageWindow.h"
#include "GameSound.h"
#include <Sphere.h>
class Cage
{
public:
	Cage(std::shared_ptr<aetherClass::FbxModel>, aetherClass::Vector3 position, aetherClass::ViewCamera*, bool);
	~Cage();

	void mUpdate(const float timeScale,aetherClass::Vector3,const bool button);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mUIRender(aetherClass::ShaderBase*);
	void mSetIsTought(bool);
	std::shared_ptr <aetherClass::Sphere>& mGetCollider();
	void mSetIsComment(bool);

	void mSetMessagePath(int,std::string);
	aetherClass::Vector3 mGetPosition();
	bool mGetMessageRun();
	void mRegisterIcon(std::string);
private:

	void mInitialize(std::shared_ptr<aetherClass::FbxModel>, aetherClass::Vector3 position, aetherClass::ViewCamera*,bool);
	void mFinalize();

	void mCreatePrisoner();
private:
	std::shared_ptr<aetherClass::FbxModel> m_model;
	std::shared_ptr<aetherClass::FbxModel> m_cage;
	std::shared_ptr<aetherClass::ModelBase> m_commentFlame;
	std::shared_ptr<aetherClass::Texture> m_pMessage;
	MessageWindow m_messageWindow;
	aetherClass::ViewCamera* m_camera;
	aetherClass::Vector3 m_initialPosition;
	CharaEntity m_charaEntity;
	std::shared_ptr < aetherClass::Sphere> m_pCollider;
	std::array<std::string, 2> m_messagePath; 
	aetherClass::Texture m_buttonTexture;
	bool m_isTought;
	bool m_isComment;
	bool m_isMessage;
	float m_changeCommentCount;
	bool m_changeComment;
	int m_animationCount;
	aetherClass::GameSound m_buttonSE;
	aetherClass::Texture m_icon;

};

#endif