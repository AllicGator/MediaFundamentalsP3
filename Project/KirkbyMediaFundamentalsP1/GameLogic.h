#pragma once

/*	Project:	KirkbyMediaFundamentalsP1
 *	File:		cGameEntity.h
 *	Author:		Dylan Kirkby
 *	Purpose:	Consolidating my games includes and some globals
 *				Ideally several gameplay functions ould be in here 
 *				as well, but they depend the AudioItem class/vector
 *				I can't figure out how to remove from main.cpp.
 */

#include "cGameEntity.h"
#include "cBoss.h"
#include "cSkeleton.h"
#include "cPlayer.h"

extern bool _toggle_help;
extern bool _game_won;
extern bool _game_lost;

extern int _info_item;
extern int _skeleton_index;
extern int _skeleton_max;
