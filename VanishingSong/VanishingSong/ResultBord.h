#ifndef _RESULTBORD_H
#define _RESULTBORD_H
#include <SpriteBase.h>
#include <ShaderBase.h>
#include "GameManager.h"
#include <array>
#include <memory>
#include <unordered_map>
namespace{
	const int kMaxGetNote = 5; // �P���̉����̍ő�擾��
	const int kMaxNumber = 2;  // �����̉摜�͑����Ă��񖇂ł���H������
}

// 
struct ResultData{
	ResultData(){
		mReset();
	}
	~ResultData(){
		mReset();
	}
	int _killEnemy; // �|�����G�̐�
	int _getNote; // �擾���������̐���ۑ�
	int _maxCombo; // �ő�R���{��

	void mReset(){
		_killEnemy = 0; // �|�����G�̐�
		_getNote = 0; // �擾���������̐���ۑ�
		_maxCombo = 0; // �ő�R���{��
	}
};

class ResultBord
{
public:
	enum class eClickState{
		eNextDay,
		eExit,
		eNull
	};
public:
	ResultBord();
	~ResultBord();
	void mInitialize();
	void mSetupData(ResultData);
	eClickState mUpdate(aetherClass::Vector2 mouse, bool isClick);
	void mRender(aetherClass::ShaderBase*,aetherClass::ShaderBase*);
	void mReset();
	void mFinalize();
	bool mFadeIn();
	bool mFadeOut();
private:

	void mAttachTexture(std::array<std::shared_ptr<aetherClass::SpriteBase>, kMaxNumber>&,const int count);
	void RegisterTexture(eClickState, std::string texturePath);
private:
	float m_updateTime; // �X�V�񐔕ۑ��p
	std::array<std::shared_ptr<aetherClass::SpriteBase>, kMaxNumber> m_pKill;
	std::array<std::shared_ptr<aetherClass::SpriteBase>, kMaxNumber> m_pCombo;
	std::array<std::unique_ptr<aetherClass::SpriteBase>, kMaxGetNote> m_pNote; // �擾���������̕\���p
	std::unique_ptr<aetherClass::SpriteBase> m_pClickObject;
	std::unique_ptr<aetherClass::SpriteBase> m_pMainBord;
	std::unique_ptr<aetherClass::Texture> m_pMainBordTexture;
	std::unique_ptr<aetherClass::SpriteBase> m_pBlurObject;

	std::unordered_map<eClickState, std::shared_ptr<aetherClass::Texture>> m_pClickTexture;
	ResultData m_data;

	bool m_isSetup;
	bool m_callFadeIn;
	bool m_callFadeOut;
};

#endif