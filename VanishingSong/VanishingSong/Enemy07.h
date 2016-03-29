#pragma once
#include "Enemy.h"
class Enemy07 :	public Enemy{

	int init();
	int move_enemy();

public:
	Enemy07(int,int);
	~Enemy07();
};

