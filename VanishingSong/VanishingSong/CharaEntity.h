#ifndef _CHARAENTITY_H
#define _CHARAENTITY_H

#include <memory>
#include "GearFrame.h"

class CharaEntity
{
public:
	CharaEntity();
	~CharaEntity();
	 std::shared_ptr<Gear> mSetUpGear(std::string path, Gear::eType, aetherClass::ViewCamera*);
	void mCreateRelationship(std::shared_ptr<Gear> parentGear, std::shared_ptr<Gear> child);
	void mGearRender(std::shared_ptr<Gear> gear, aetherClass::ShaderBase*);

};


#endif