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
	

	// ���̓��ɐi�ނƂ��̏���������
	void mNextDayInitialize(GameManager::eDay nextDay)override;

	// �O�̓��̉������
	void mPrevDayFinalize(GameManager::eDay prevDay)override;

private:
	void mUpdate(const float timeScale, const float nowTime) override;
	void mRender(ShaderHash shader)override;
	void mUIRender(ShaderHash shader)override;
};

#endif