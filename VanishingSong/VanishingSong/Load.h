#ifndef LOAD_H
#define LOAD_H
#include <LoadScreen.h>
#include <memory>
#include <SpriteBase.h>
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
	float m_changeBarTime;
	int m_barCount;
};

#endif