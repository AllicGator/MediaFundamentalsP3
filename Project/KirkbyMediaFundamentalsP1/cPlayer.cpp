/*	Project:	KirkbyMediaFundamentalsP1
 *	File:		cPlayer.cpp
 *	Author:		Dylan Kirkby
 *	Purpose:	Implementation for cPlayer.h
 */

#include "cPlayer.h"

Player::Player()
{
	srand(time(NULL));
	maxhp = 100;
	curhp = maxhp;
	minDmg = rand() % 8 + 5;
	maxDmg = rand() % 20 + 9;
	hitRate = rand() % 90 + 80;
}

void Player::Heal()
{
	curhp += 50;
	if (curhp > maxhp)
		curhp = maxhp;
}
