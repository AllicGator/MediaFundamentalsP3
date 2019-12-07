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
extern std::vector<cAudioItem*> vec_NetSound;
extern std::vector<cAudioItem*> vec_TTS_WAV;
extern std::vector<std::wstring> vec_TTS_Paragraph;

class cAudioManager {
	cAudioManager();
	static cAudioManager stonAudioMngr;

	//Filepaths
	std::string directory = "./config_files/";
	std::string netSounds = "netSounds.xml";
	std::string textToSpeech = "textToSpeech.xml";
	std::string textToWav = "textToWav.xml";

public:
	FMOD::System* _system;
	FMOD_RESULT _result;
	static cAudioManager* GetAudioManager();


	void LoadDSPs();
	bool LoadNetSounds();
	bool LoadTextToSpeech();
	bool LoadTextToWav();
	
	void Release();

	void error_check();
};
