#pragma once

/*	Project:	KirkbyMediaFundamentalsP1
 *	File:		cBoss.h
 *	Author:		Dylan Kirkby
 *	Purpose:	Simple Derived class for the games boss 
 *				- should realistically combine it with skeleton into 
 *				an enemy class that takes variables in the constructor
 */

#include "cGameEntity.h"

class Boss : public GameEntity
{
public:
	std::vector<std::string> asciiArt;
	Boss(std::string path);
};