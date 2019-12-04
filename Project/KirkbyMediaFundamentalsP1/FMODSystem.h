#pragma once

/*	Project:	KirkbyMediaFundamentalsP1
 *	File:		FMODSystem.h
 *	Author:		Dylan Kirkby
 *	Purpose:	An attempt at removing more FMOD stuff from main
 *				ran into a lot of circular dependancies I couldn't figure out at this point.
 */

#include <fmod.hpp>
#include <fmod_errors.h>
#include <map>

extern FMOD::System* _system;
extern FMOD_RESULT _result;

//FMOD Enum Conversion Maps
extern std::map<FMOD_SOUND_FORMAT, const char*> _map_format;
extern std::map<FMOD_SOUND_TYPE, const char*> _map_type;
