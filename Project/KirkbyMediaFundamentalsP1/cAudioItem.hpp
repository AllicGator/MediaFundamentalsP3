#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>
#include <map>

extern std::map<std::string, FMOD::ChannelGroup*> mpChannelGoups;

class cAudioItem
{
private:
	FMOD::System* _system;
	FMOD::Sound* sound;
	FMOD::Channel* channel;
	FMOD::ChannelGroup* channel_group;
	FMOD_RESULT _result;
	std::string group_name;
	std::string path;
	std::string friendlyName;
	float volume;
	float pitch;
	float pan;
	bool is_paused;
	bool is_playing;
	unsigned int pos;
	unsigned int length;

public:
	cAudioItem(FMOD::System* system);
	~cAudioItem();

	//Getters
	std::string GetPath();
	FMOD::ChannelGroup* GetChannelGroup();
	FMOD::Channel* GetChannel();
	FMOD::Sound* GetSound();
	float GetPan();

	//Setters	
	void SetPan(float num);
	void SetPath(std::string file);
	void SetChannel(int ID);
	void SetChannelGroup(std::string name);
	void SetIsPlaying(bool play);
	void SetIsPaused(bool pause);

	bool CreateSound(bool is_streamed_sound);
	bool PlaySound();

	std::string get_info();
	void error_check();
};