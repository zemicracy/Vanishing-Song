#pragma once
#include "Mode.h"
class ModePractice :
	public Mode
{
public:
	ModePractice();
	~ModePractice();

	bool mInitialize(GameManager::eSkillType skill, GameManager::eDay firstDay)override;


	// 次の日に進むときの初期化処理
	void mNextDayInitialize(GameManager::eDay nextDay)override;

	// 前の日の解放処理
	void mPrevDayFinalize(GameManager::eDay prevDay)override;

private:
	void mUpdate(const float timeScale, const float nowTime) override;
	void mRender(ShaderHash shader)override;
	void mUIRender(ShaderHash shader)override;
};

