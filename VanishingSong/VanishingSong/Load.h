#ifndef LOAD_H
#define LOAD_H
#include <LoadScreen.h>
#include <memory>
#include <SpriteBase.h>
#include <FbxModel.h>
class Load :
	public aetherClass::LoadScreen
{
public:
	Load();
	~Load();

	void Initialize()override;
	void Finalize()override;
	void Run()override;
	bool WaitRun()override;

private:
	void mChangeBar(float& time);
	void mResetProperty();
private:
	std::unique_ptr<aetherClass::SpriteBase> m_pLoadBar;
	std::shared_ptr<aetherClass::FbxModel> m_model;
	float m_changeBarTime;
	int m_barCount;
	aetherClass::ViewCamera m_view;
};

#endif