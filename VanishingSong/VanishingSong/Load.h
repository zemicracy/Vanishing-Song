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
	std::unique_ptr<aetherClass::SpriteBase> m_pLoadMain;
	std::unique_ptr<aetherClass::Texture> m_pLoadMainTexture;
};

#endif