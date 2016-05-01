#ifndef LOAD_H
#define LOAD_H
#include <LoadScreen.h>
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
};

#endif