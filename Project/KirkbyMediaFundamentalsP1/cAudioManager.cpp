#include "cAudioManager.hpp"
#include <iostream>
#include <fstream>
#include <RapidXml\rapidxml.hpp>

#pragma region SINGLETON
cAudioManager cAudioManager::stonAudioMngr;
cAudioManager* cAudioManager::GetAudioManager() { return &(cAudioManager::stonAudioMngr); }
cAudioManager::cAudioManager() {
	std::cout << "Audio Manager Created" << std::endl;
	_result = FMOD_OK;

	//Create system
	_result = FMOD::System_Create(&_system);
	this->error_check();

	//Init system
	_result = _system->init(32, FMOD_INIT_NORMAL, 0);
	this->error_check();

	_result = _system->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
	this->error_check();
}
#pragma endregion

#pragma region Loaders
void cAudioManager::LoadDSPs() {
	fprintf(stdout, "Loading DSPs...\n");

	g_vec_DSP.push_back(DSP());
	_result = _system->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
	this->error_check();

	g_vec_DSP.push_back(DSP());
	_result = _system->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
	this->error_check();

	g_vec_DSP.push_back(DSP());
	_result = _system->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
	this->error_check();

	g_vec_DSP.push_back(DSP());
	_result = _system->createDSPByType(FMOD_DSP_TYPE_ITECHO, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
	this->error_check();

	g_vec_DSP.push_back(DSP());
	_result = _system->createDSPByType(FMOD_DSP_TYPE_FLANGE, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
	this->error_check();

	g_vec_DSP.push_back(DSP());
	_result = _system->createDSPByType(FMOD_DSP_TYPE_NORMALIZE, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
	this->error_check();

	g_vec_DSP.push_back(DSP());
	_result = _system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
	this->error_check();

	g_vec_DSP.push_back(DSP());
	_result = _system->createDSPByType(FMOD_DSP_TYPE_ECHO, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
	this->error_check();

	g_vec_DSP.push_back(DSP());
	_result = _system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
	this->error_check();
}

bool cAudioManager::LoadNetSounds() {
	fprintf(stdout, "Loading Net Sounds...\n");

	rapidxml::xml_document<> document;
	rapidxml::xml_node<>* root_node;
	std::string path = this->directory + this->netSounds;

	// Read XML file
	std::ifstream soundFiles(path);
	if (!soundFiles) {
		printf("Unable to open %s\n", path.c_str());
		system("pause");
		exit(1);
	}

	// Copy the file contents into our buffer
	std::vector<char> buffer((std::istreambuf_iterator<char>(soundFiles)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml parsinglib
	document.parse<0>(&buffer[0]);
	// Find the root node
	root_node = document.first_node("SoundFiles");

	// Iterate through all of the root node's children
	// use to build audio object
	for (rapidxml::xml_node<>* platform_node = root_node->first_node("File");
		platform_node;
		platform_node = platform_node->next_sibling()) {
		int i = 0;
		if (platform_node->first_attribute("path")->value() > 0) {
			cAudioItem ai(this->_system);
			ai.SetPath(platform_node->first_attribute("path")->value());
			/*if (*platform_node->last_attribute("stream")->value() == 't')
				ai.create_and_play_sound(true, i++);
			else
				ai.create_and_play_sound(false, i++);*/
			////_audio_items.push_back(ai);
		}
	}

	return true;
}

bool cAudioManager::LoadTextToSpeech() {
	fprintf(stdout, "Loading Text to Speech...\n");
	rapidxml::xml_document<> document;
	rapidxml::xml_node<>* root_node;

	std::string path = this->directory + this->textToSpeech;

	// Read XML file
	std::ifstream soundFiles(path);
	if (!soundFiles) {
		printf("Unable to open %s\n", path.c_str());
		system("pause");
		exit(1);
	}

	// Copy the file contents into our buffer
	std::vector<char> buffer((std::istreambuf_iterator<char>(soundFiles)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml parsinglib
	document.parse<0>(&buffer[0]);
	// Find the root node
	root_node = document.first_node("SoundFiles");

	// Iterate through all of the root node's children
	// use to build audio object
	for (rapidxml::xml_node<>* platform_node = root_node->first_node("File");
		platform_node;
		platform_node = platform_node->next_sibling()) {
		int i = 0;
		if (platform_node->first_attribute("path")->value() > 0) {
			cAudioItem ai(this->_system);
			ai.SetPath(platform_node->first_attribute("path")->value());
			/*if (*platform_node->last_attribute("stream")->value() == 't')
				ai.create_and_play_sound(true, i++);
			else
				ai.create_and_play_sound(false, i++);*/
				////_audio_items.push_back(ai);
		}
	}

	return true;
}

bool cAudioManager::LoadTextToWav() {
	fprintf(stdout, "Loading Text to .Wav...\n");
	rapidxml::xml_document<> document;
	rapidxml::xml_node<>* root_node;

	std::string path = this->directory + this->textToWav;

	// Read XML file
		std::ifstream soundFiles(path);
	if (!soundFiles) {
		printf("Unable to open %s\n", path.c_str());
		system("pause");
		exit(1);
	}

	// Copy the file contents into our buffer
	std::vector<char> buffer((std::istreambuf_iterator<char>(soundFiles)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml parsinglib
	document.parse<0>(&buffer[0]);
	// Find the root node
	root_node = document.first_node("SoundFiles");

	// Iterate through all of the root node's children
	// use to build audio object
	for (rapidxml::xml_node<>* platform_node = root_node->first_node("File");
		platform_node;
		platform_node = platform_node->next_sibling()) {
		int i = 0;
		if (platform_node->first_attribute("path")->value() > 0) {
			cAudioItem ai(this->_system);
			ai.SetPath(platform_node->first_attribute("path")->value());
			/*if (*platform_node->last_attribute("stream")->value() == 't')
				ai.create_and_play_sound(true, i++);
			else
				ai.create_and_play_sound(false, i++);*/
				////_audio_items.push_back(ai);
		}
	}

	return true;
}
#pragma endregion

void cAudioManager::Release() {
	printf("Releasing FMOD Members\n");
	//remove DSPs from channel groups
	//DEPENDANT ON ALL 9 EXISTING
	for (std::pair<std::string, FMOD::ChannelGroup*> member : mpChannelGoups) {
		FMOD::DSP* tmpDSP;
		for (size_t i = 0; i < 3; i++) {
			member.second->getDSP(0, &tmpDSP);
			this->error_check();
			member.second->removeDSP(tmpDSP);
			this->error_check();
		}
	}

	//release DSPs
	for (DSP member : g_vec_DSP) {
		if (member.dsp) {
			_result = member.dsp->release();
			this->error_check();
		}
	}
	g_vec_DSP.clear();

	//delete audio items, which handle their own cleanup.
	for (std::map<std::string, cAudioItem*>::iterator mapIt = mpAudio.begin();
		mapIt != mpAudio.end();
		mapIt++) {
		delete mapIt->second;
	}

	_result = _system->close();
	this->error_check();
	_result = _system->release();
	this->error_check();
}

void cAudioManager::error_check() {
	if (this->_result != FMOD_OK) {
		//error.DisplayError("cAudioManager", __func__, "FMOD Error", FMOD_ErrorString(this->_result));
		std::cout << "cAudioManager " << __func__ << std::endl << "FMOD Error: " << FMOD_ErrorString(this->_result) << std::endl;
		exit(1);
	}
}
