#pragma once
#include <vector>
#include <map>
#include "cAudioItem.hpp"
#include <fmod.hpp>
#include <fmod_errors.h>
#include <map>
#include <string>

struct DSP {
	FMOD::DSP* dsp;
	FMOD::ChannelGroup* channelgroup;
	bool				bypass;
};

extern std::map<std::string, FMOD::ChannelGroup*> mpChannelGoups;
extern std::map<std::string, cAudioItem*> mpAudio;
extern std::vector<DSP> g_vec_DSP;

class cAudioManager {
	cAudioManager();
	static cAudioManager stonAudioMngr;

public:
	FMOD::System* _system;
	FMOD_RESULT _result;

	static cAudioManager* GetAudioManager();

	void LoadDSPs();

	void Release();

	void error_check();
};
