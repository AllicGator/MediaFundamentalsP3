#include "cAudioItem.hpp"
#include <iostream>
#include <sstream>

void cAudioItem::error_check() {
	if (this->_result != FMOD_OK) {
		std::cout << "cAudioItem " << __func__ << std::endl << "FMOD Error: " << FMOD_ErrorString(this->_result) << std::endl;
		exit(1);
	}
}

cAudioItem::cAudioItem(FMOD::System* system) {

	/*Audio Object*/
	this->_system = system;
	this->_result = FMOD_OK;
	this->sound = 0;
	this->channel = 0;
	this->channel_group = 0;
	this->group_name = "";
	this->volume = 1.0f;
	this->pitch = 1.0f;
	this->pan = 0.0f;
	this->is_paused = true;
	this->is_playing = false;
	this->path = "";
}

cAudioItem::~cAudioItem() {
	if (this->sound) {
		this->_result = this->sound->release();
		this->error_check();
	}
	if (this->channel_group
		&& mpChannelGoups.find(group_name) != mpChannelGoups.end()) {
		mpChannelGoups.erase(this->group_name);
		this->_result = this->channel_group->release();
		this->error_check();
	}
}

std::string cAudioItem::GetPath()
{
	return this->path;
}

FMOD::ChannelGroup* cAudioItem::GetChannelGroup() {
	return this->channel_group;
}

FMOD::Channel* cAudioItem::GetChannel() {
	return this->channel;
}

FMOD::Sound* cAudioItem::GetSound() {
	return this->sound;
}

float cAudioItem::GetPan()
{
	return this->pan;
}

void cAudioItem::SetPan(float num)
{
	this->pan = num;
}

void cAudioItem::SetPath(std::string file) {
	this->path = file;
}

void cAudioItem::SetChannel(int ID)
{
	this->_result = this->_system->getChannel(ID, &this->channel);
	this->error_check();
}

void cAudioItem::SetChannelGroup(std::string name) {
	if (mpChannelGoups.find(name) == mpChannelGoups.end()) {
		this->_result = this->_system->createChannelGroup(name.c_str(), &this->channel_group);
		this->error_check();
		mpChannelGoups[name.c_str()] = this->channel_group;
	}
	else {
		this->channel_group = mpChannelGoups[name];
		this->error_check();
	}
	this->group_name = name;
	this->_result = this->channel->setChannelGroup(this->channel_group);
	this->error_check();

	this->channel_group->setPaused(this->is_paused);
	this->error_check();
}

void cAudioItem::SetIsPlaying(bool play) {
	this->is_playing = false;
}

void cAudioItem::SetIsPaused(bool pause) {
	this->is_paused = pause;
}

bool cAudioItem::CreateSound(bool is_streamed_sound) {
	this->_result = this->_system->createSound(this->path.c_str(), (is_streamed_sound) ? FMOD_LOOP_NORMAL | FMOD_CREATESTREAM : FMOD_DEFAULT, 0, &this->sound);
	this->error_check();

	PlaySound();

	return true;
}

bool cAudioItem::PlaySound() {
	this->_result = this->_system->playSound(this->sound, this->channel_group, this->is_paused, &this->channel);
	this->error_check();
	return true;
}

std::string cAudioItem::get_info() {
	std::ostringstream ss;
	ss.precision(2);
	ss << "Name: " << this->friendlyName << " Volume: " << this->volume << " | Pan: " << this->pan << " | Pitch: " << this->pitch << " | Paused: " << ((this->is_paused) ? "YES" : "NO") << " | Playing: " << ((this->is_playing) ? "YES" : "NO");
	return ss.str();
}