#pragma once

/*	Project:	KirkbyMediaFundamentalsP1
 *	File:		cSkeleton.h
 *	Author:		Dylan Kirkby
 *	Purpose:	Simple Derived class for the skeleton enemy type
 *				- should realistically combine it with boss class to make
 *				an enemy class that takes variables in the constructor
 */

#include "cGameEntity.h"

class Skeleton : public GameEntity
{
public:
	std::vector<std::string> asciiArt;
	Skeleton(std::string path);
};