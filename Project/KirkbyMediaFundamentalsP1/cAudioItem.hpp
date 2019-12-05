#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>
#include <map>

extern std::map<std::string, FMOD::ChannelGroup*> mpChannelGoups;
const int STRING_BUFFER_SIZE = 255;
const int STREAM_BUFFER_SIZE_IN_MB = (64 * 1024); const int _tag_count = 4;

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
	unsigned int percent;
	bool is_starving;
	const char* state;

	//TODO2: FMOD NET STREAM SPECIFICS
	int _tag_index = 0;
	char _tag_string[_tag_count][STRING_BUFFER_SIZE];
	FMOD_OPENSTATE _open_state = FMOD_OPENSTATE_READY;
	FMOD_TAG _tag;
	FMOD_CREATESOUNDEXINFO _ei;

public:
	cAudioItem(FMOD::System* system);
	~cAudioItem();

	//Getters
	std::string GetPath();
	FMOD::ChannelGroup* GetChannelGroup();
	FMOD::Channel* GetChannel();
	FMOD::Sound* GetSound();
	float GetPan();
	FMOD_OPENSTATE GetOpenState();
	FMOD_CREATESOUNDEXINFO GetExtendedInfo();
	const char* GetStateString();
	unsigned int* GetPosition();
	bool* GetIsPaused();
	bool* GetIsPlaying();
	bool* GetIsStarving();
	std::string GetGroupName();

	//Setters	
	void SetPan(float num);
	void SetPath(std::string file);
	void SetChannel(int ID);
	void SetChannelGroup(std::string name);
	void SetIsPlaying(bool play);
	void SetIsPaused(bool pause);
	void SetExtendedInfo(FMOD_CREATESOUNDEXINFO _ei);
	void SetState(const char* st);
	void SetGroupName(std::string name);

	bool CreateSound(bool is_streamed_sound);
	bool CreateNetSound();
	bool PlaySoundEh();

	bool PlayNetSound();

	std::string get_info();
	void error_check();
};