#include "cAudioManager.hpp"
#include <iostream>

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

#pragma region DSPs
void cAudioManager::LoadDSPs() {
	g_vec_DSP.push_back(DSP());
	_result = _system->createDSPByType(FMOD_DSP_TYPE_ECHO, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
	this->error_check();

	g_vec_DSP.push_back(DSP());
	_result = _system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
	this->error_check();

	g_vec_DSP.push_back(DSP());
	_result = _system->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
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
	_result = _system->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
	this->error_check();

	g_vec_DSP.push_back(DSP());
	_result = _system->createDSPByType(FMOD_DSP_TYPE_ITLOWPASS, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
	this->error_check();

	g_vec_DSP.push_back(DSP());
	_result = _system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &g_vec_DSP[g_vec_DSP.size() - 1].dsp);
	this->error_check();
}
#pragma endregion

void cAudioManager::Release() {
	printf("Releasing FMOD Members\n");
	//remove DSPs from channel groups
	//DEPENDANT ON ALL 9 EXISTING
	/*for (std::pair<std::string, FMOD::ChannelGroup*> member : mpChannelGoups) {
		FMOD::DSP* tmpDSP;
		for (size_t i = 0; i < 3; i++) {
			member.second->getDSP(0, &tmpDSP);
			this->error_check();
			member.second->removeDSP(tmpDSP);
			this->error_check();
		}
	}*/

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
