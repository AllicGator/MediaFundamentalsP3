#pragma once

/*	Project:	KirkbyMediaFundamentalsP1
 *	File:		cPlayer.h
 *	Author:		Dylan Kirkby
 *	Purpose:	The player!
 *				Has a heal function so that you can reliably beat the boss
 */

#include "cGameEntity.h"

class Player : public GameEntity
{
public:
	Player();
	void Heal();
};