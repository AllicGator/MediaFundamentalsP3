#pragma once

/*	Project:	KirkbyMediaFundamentalsP1
 *	File:		cGameEntity.h
 *	Author:		Dylan Kirkby
 *	Purpose:	Base class for ease of constructing my player and enemies.
 */

#include <time.h>
#include <stdlib.h>
#include <string>
#include <vector>

class GameEntity
{
protected:
	int curhp;
	int maxhp;
	int minDmg;
	int maxDmg;
	int hitRate;

public:
	GameEntity();
	int GetCurHP();
	int GetMaxHP();
	int DealDamag();
	bool TakeDamage(int dmg);
};