#ifndef _ANIMATION_H
#define _ANIMATION_H
#include "Gear.h"

struct Animation
{
	aetherClass::Transform _start;
	aetherClass::Transform _end;
	Gear::eType _name;
};

#endif