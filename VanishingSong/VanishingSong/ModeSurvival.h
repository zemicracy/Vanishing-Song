#ifndef _MODESUVIVAL_H
#define _MODESUVIVAL_H
#include "Mode.h"
class ModeSurvival :
	public Mode
{
public:
	ModeSurvival();
	~ModeSurvival();

	bool mInitialize(GameManager::eSkillType skill, GameManager::eDay firstDay)override;
	

	// Ÿ‚Ì“ú‚Éi‚Ş‚Æ‚«‚Ì‰Šú‰»ˆ—
	void mNextDayInitialize(GameManager::eDay nextDay)override;

	// ‘O‚Ì“ú‚Ì‰ğ•úˆ—
	void mPrevDayFinalize(GameManager::eDay prevDay)override;

private:
	void mUpdate(const float timeScale, const float nowTime) override;
	void mRender(ShaderHash shader)override;
	void mUIRender(ShaderHash shader)override;
};

#endif