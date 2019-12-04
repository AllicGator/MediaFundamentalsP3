/*	Project:	KirkbyMediaFundamentalsP1
 *	File:		cGameEntity.cpp
 *	Author:		Dylan Kirkby
 *	Purpose:	Implementation for cGameEntity.h
 */

#include "cGameEntity.h"

GameEntity::GameEntity()
{
	maxhp = 0;
	curhp = maxhp;
	minDmg = 0;
	maxDmg = 0;
	hitRate = 0;
}

int GameEntity::GetCurHP()
{
	return curhp;
}

int GameEntity::GetMaxHP()
{
	return maxhp;
}

int GameEntity::DealDamag()
{
	srand(time(NULL));
	int hit = rand() % 100 + 1;
	if (hit < hitRate)
		return rand() % maxDmg + minDmg;
	return 0;
}

bool GameEntity::TakeDamage(int dmg)
{
	int tmphp = curhp;
	curhp -= dmg;
	if (curhp == tmphp)
		return false;
	return true;
}
