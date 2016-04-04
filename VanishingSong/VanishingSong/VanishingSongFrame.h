#ifndef _VANISHINGSONGFRAME_H
#define _VANISHINGSONGFRAME_H
#include <GameFrame.h>
class VanishingSongFrame:
	public aetherClass::GameFrame
{
public:
	VanishingSongFrame();
	~VanishingSongFrame();

private:
	bool InitializeBuffer()override;

	bool FrameRunningBuffer()override;

	void FinalizeBuffer()override;
};

#endif