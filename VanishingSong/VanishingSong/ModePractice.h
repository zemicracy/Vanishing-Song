#pragma once
#include "Mode.h"
class ModePractice :
	public Mode
{
public:
	ModePractice();
	~ModePractice();

	bool mInitialize(GameManager::eDay firstDay)override;
	void mRender(ShaderHash shader)override;

	// ���̓��ɐi�ނƂ��̏���������
	void mNextDayInitialize(GameManager::eDay nextDay)override;

	// �O�̓��̉������
	void mPrevDayFinalize(GameManager::eDay prevDay)override;

private:
	void mUpdate(std::shared_ptr<ActionCommand> command, const float timeScale, const float nowTime) override;
};

