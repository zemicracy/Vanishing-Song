#ifndef _ResultBoard_H
#define _ResultBoard_H
#include<SpriteBase.h>
#include <ShaderBase.h>
#include "GameManager.h"
#include <array>
#include <memory>
#include"ClearGauge.h"
// 
struct ResultData{
	ResultData(){
		mReset();
	}
	~ResultData(){
		mReset();
	}
	int _missCount; //
	int _maxCount; // 最大コンボ数
	void mReset(){
		_missCount = 0; // 取得した音符の数を保存
		_maxCount = 0; // 最大コンボ数
	}
};

class ResultBoard
{
public:
	enum eResultState{
		eNone,eInit, eResultTitle, eMissCnt, eClearGauge, eRate, eNote, eEnd
	};

public:
	ResultBoard();
	~ResultBoard();
	void mInitialize();
	void mUpdate();
	void mSetResultData(ResultData,GameManager::eBattleState,UINT);
	void mRender(aetherClass::ShaderBase*,aetherClass::ShaderBase*);

private:
	void mFinalize();

	std::unordered_map < std::string, std::shared_ptr<aetherClass::SpriteBase>>m_pGeneral;
	std::shared_ptr<ClearGauge>m_pGauge;
	std::shared_ptr<aetherClass::SpriteBase>m_pNumSprite;

	aetherClass::Vector3 m_rankRatePosision;
	aetherClass::Vector3 m_missCountPosision;

	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>>m_numberList;
	std::unordered_map<std::string, std::shared_ptr<aetherClass::Texture>>m_TextureList;

	std::string m_rateString;
	std::shared_ptr<HalfFillShader>m_halfFill;
	ResultData m_resultData;
	bool m_isSetup;
	bool m_callFadeIn;
	bool m_callFadeOut;
	float m_timer;
	eResultState m_state;

};

#endif