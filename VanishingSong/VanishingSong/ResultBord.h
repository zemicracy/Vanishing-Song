#ifndef _RESULTBORD_H
#define _RESULTBORD_H
#include <SpriteBase.h>
#include <ShaderBase.h>
#include "GameManager.h"
#include <array>
#include <memory>
#include <unordered_map>
namespace{
	const int kMaxGetNote = 5; // １日の音符の最大取得数
	const int kMaxNumber = 2;  // 数字の画像は多くても二枚でしょ？ｗｗｗ
}

// 
struct ResultData{
	ResultData(){
		mReset();
	}
	~ResultData(){
		mReset();
	}
	int _killEnemy; // 倒した敵の数
	int _getNote; // 取得した音符の数を保存
	int _maxCombo; // 最大コンボ数

	void mReset(){
		_killEnemy = 0; // 倒した敵の数
		_getNote = 0; // 取得した音符の数を保存
		_maxCombo = 0; // 最大コンボ数
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
	float m_updateTime; // 更新回数保存用
	std::array<std::shared_ptr<aetherClass::SpriteBase>, kMaxNumber> m_pKill;
	std::array<std::shared_ptr<aetherClass::SpriteBase>, kMaxNumber> m_pCombo;
	std::array<std::unique_ptr<aetherClass::SpriteBase>, kMaxGetNote> m_pNote; // 取得した音符の表示用
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