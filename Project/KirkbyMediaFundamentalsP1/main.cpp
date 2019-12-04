///*
//==================================================================================================================================================================
//|Copyright 2019 Oscar Lara	- scarlara@hotmail.com																												   |
//|																																								   |
//|See individual libraries separate legal notices																												   |
//|																																								   |
//|Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"),				   |
//|to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,				   |
//|and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :					   |
//|																																								   |
//|The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.									   |
//|																																								   |
//|THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			   |
//|FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,	   |
//|WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.		   |
//==================================================================================================================================================================
//*/
//

/*	Project:	KirkbyMediaFundamentalsP1
 *	File:		main.cpp
 *	Author:		Dylan Kirkby
 *	Co-Authur:	Oscar Lara
 *	Purpose:	Expand my knowledge on the basics of FMOD by
 *				making a basic sound manipulation program
 *				with simplistic gameplay elements.
 *				Should be heavily refactored yet, but I was
 *				having a lot of circular dependancy logic problems.
 */

#pragma region Includes
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <RapidXml/rapidxml.hpp>

#include "FMODSystem.h"
#include "GameLogic.h"

#include <ft2build.h>
#include <iostream>
#include FT_FREETYPE_H  

#pragma endregion

#pragma region Globals
//Globals
unsigned int _vertex_shader, _fragment_shader, _program;
GLfloat _current_y_position = 0.0f;
GLfloat _y_offset = 30.0f;
char _text_buffer[512];

//GLFW
int _window_width = 640;
int _window_height = 480;
GLFWwindow* _main_window = NULL;

FT_Library _ft;
FT_Face _face;
unsigned int _text_program;
unsigned int _uniform_tex;
unsigned int _attribute_coord;
unsigned int _dp_vbo;
unsigned int _uniform_color;


Player _player;
Boss _boss = Boss("../common/assets/ascii/death.txt");
std::vector<Skeleton> _skeletons;
#pragma endregion

#define BUFFER_SIZE 255

//FMOD globals
//FMOD functions
void errorCheck(FMOD_RESULT result);
void releaseFMOD();
bool initFMOD();

class AudioItem
{
private:
	FMOD_RESULT result;
	std::string path;
	std::string format;
	std::string type;
	char name[BUFFER_SIZE];
	float volume;	//Volume level, from 0.0 to 1.0
	float pitch;	//Pitch value, 0.5 = half pitch, 2.0 = double pitch
	float pan; //Pan level, from -1.0 (left) to 1.0 (right), default = 0 (center).
	float freq;
	bool is_paused;
	bool is_playing;
	int channels;
	int bits;
	unsigned int pos;
	unsigned int length;

public:
	FMOD::Sound* sound;
	FMOD::Channel* channel;
	//constructor
	AudioItem() {
		path = "";
		name[BUFFER_SIZE - 1] = { '\0' };
		volume = 1.0f;
		pitch = 1.0f;//check documentation for pitch allowed values
		pan = 0.0f; //chec documentation for pan allowed values
		is_paused = true;
		is_playing = false;
		sound = 0;
		channel = 0;
	}

	bool create_and_play_sound(bool is_streamed_sound, int channelID) {
		if (_system) 
		{
			result = _system->createSound(path.c_str(), (is_streamed_sound) ? FMOD_CREATESTREAM : FMOD_DEFAULT, 0, &sound);
			errorCheck(result);

			result = _system->getChannel(channelID, &channel);
			errorCheck(result);

			result = _system->playSound(sound, 0, is_paused, &channel);
			errorCheck(result);

			if (is_streamed_sound && channel)
			{
				result = channel->setMode(FMOD_LOOP_NORMAL);
				errorCheck(result);
			}

			if (sound) 
			{
				FMOD_SOUND_TYPE tmpType;
				FMOD_SOUND_FORMAT tmpFormat;
				result = sound->getFormat(&tmpType, &tmpFormat, &channels, &bits);
				errorCheck(result);

				format = _map_format[tmpFormat];
				type = _map_type[tmpType];
			}

			return true;
		}

		return false;
	}

	void restart_sound()
	{
		if (_system)
		{
			float tmpPitch = pitch;
			float tmpVolume = volume;
			channel->stop();
			result = _system->playSound(sound, 0, false, &channel);
			set_volume(tmpVolume);
			set_pitch(tmpPitch);
			errorCheck(result);
		}
	}

	std::string get_name() 
	{
		if (sound) 
		{
			result = sound->getName(name, BUFFER_SIZE);
			errorCheck(result);
		}
		return name;
	}

	float get_volume() 
	{
		if (channel) 
		{
			result = channel->getVolume(&volume);
			errorCheck(result);
		}
		return volume;
	}
	void change_volume(float incr)
	{
		if (channel)
		{
			channel->getVolume(&volume);
			//casting to double to avoid overflow warnings
			if (static_cast<double>(volume) + incr >= 1.0)
				result = channel->setVolume(1.0);
			else if (static_cast<double>(volume) + incr <= 0.0)
				result = channel->setVolume(0.0);
			else
				result = channel->setVolume(volume += incr);
			errorCheck(result);
		}
	}
	void set_volume(float vol)
	{
		if (channel)
		{
			channel->getVolume(&volume);
			//casting to double to avoid overflow warnings
			if (vol >= 1.0)
				result = channel->setVolume(1.0);
			else if (vol <= 0.0)
				result = channel->setVolume(0.0);
			else
				result = channel->setVolume(vol);
			errorCheck(result);
		}
	}

	void set_path(std::string new_path) {
		path = new_path;
	}

	bool get_is_paused() 
	{
		if (channel) 
		{
			result = channel->getPaused(&is_paused);
			errorCheck(result);
		}
		return is_paused;
	}
	void set_is_paused(bool new_is_paused) 
	{
		if (channel) 
		{
			is_paused = new_is_paused;
			result = channel->setPaused(is_paused);
			errorCheck(result);
		}
	}

	float get_frequency()
	{
		if (channel) 
		{
			result = channel->getFrequency(&freq);
			errorCheck(result);
		}
		return freq;
	}

	float get_pitch()
	{
		if (channel) 
		{
			result = channel->getPitch(&pitch);
			errorCheck(result);
		}
		return pitch;
	}
	void set_pitch(float pit)
	{
		if (channel)
		{
			channel->getPitch(&pitch);
			//casting to double to avoid overflow warnings
			if (pit >= 2.0)
				result = channel->setPitch(2.0);
			else if (pit <= 0.5)
				result = channel->setPitch(0.5);
			else
				result = channel->setPitch(pit);
			errorCheck(result);
		}
	}
	void change_pitch(float incr)
	{
		if (channel)
		{
			channel->getPitch(&pitch);
			//casting to double to avoid overflow warnings
			if (static_cast<double>(pitch) + incr >= 2.0)
				result = channel->setPitch(2.0);
			else if (static_cast<double>(pitch) + incr <= 0.5)
				result = channel->setPitch(0.5);
			else
				result = channel->setPitch(pitch += incr);
			errorCheck(result);
		}
	}

	//No simple method of getting pan from channel, so will store the variable myself.
	float get_pan()
	{
		return pan;
	}
	void change_pan(float incr)
	{
		if (channel)
		{
			if (static_cast<double>(pan) + incr >= 1.0)
				result = channel->setPan(pan = 1.0);
			else if (static_cast<double>(pan) + incr <= -1.0)
				result = channel->setPan(pan = -1.0);
			else
				result = channel->setPan(pan += incr);
			if (pan == -0.0)
				pan = 0.0;
			errorCheck(result);
		}
	}

	int get_bits()
	{
		return bits;
	}
	int get_channels()
	{
		return channels;
	}
	std::string get_format()
	{
		return format;
	}
	std::string get_type()
	{
		return type;
	}

	unsigned int get_position()
	{
		if (channel)
		{
			result = channel->getPosition(&pos, FMOD_TIMEUNIT_MS);
			errorCheck(result);
		}
		return pos;
	}

	bool channel_is_playing()
	{
		bool tmp;
		result = channel->isPlaying(&tmp);
		errorCheck(result);
		return tmp;
	}

	unsigned int get_length()
	{
		if (channel)
		{
			result = sound->getLength(&length, FMOD_TIMEUNIT_MS);
			errorCheck(result);
		}
		return length;
	}
};

//global audio item objects
std::vector<AudioItem> _audio_items;
int _current_audio_item_index = 0;

bool initAudioItems()
{
	rapidxml::xml_document<> document;
	rapidxml::xml_node<>* root_node;

	char input;
	bool usercontrol = true;
	std::string path;
	do
	{
		printf("Load Compressed Sounds (Y/N): ");
		std::cin >> input;
		if (input == 'y' || input == 'Y')
		{
			usercontrol = false;
			path = "./config_files/compressed_audio_file_paths.xml";
		}
		else if (input == 'n' || input == 'N')
		{
			usercontrol = false;
			path = "./config_files/uncompressed_audio_file_paths.xml";
		}
		else
			printf("Please select an option...\n");
	} while (usercontrol);

	// Read XML file
	std::ifstream soundFiles(path);

	if (!soundFiles)
	{
		printf("Unable to open %s\n", path.c_str());
		system("pause");
		return false;
	}

	// Copy the file contents into our buffer
	std::vector<char> buffer((std::istreambuf_iterator<char>(soundFiles)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	// Parse the buffer using the xml parsinglib
	document.parse<0>(&buffer[0]);

	// Find the root node
	root_node = document.first_node("SoundFiles");

	// Iterate through all of the root node's children
	for (rapidxml::xml_node<>* platform_node = root_node->first_node("File");
		platform_node;
		platform_node = platform_node->next_sibling())
	{
		int i = 0;
		if (platform_node->first_attribute("path")->value() > 0)
		{
			AudioItem ai;
			ai.set_path(platform_node->first_attribute("path")->value());
			if (*platform_node->last_attribute("stream")->value() == 't')
				ai.create_and_play_sound(true, i++);
			else
				ai.create_and_play_sound(false, i++);
			_audio_items.push_back(ai);
		}
	}

	return true;
}

void errorCheck(FMOD_RESULT result) 
{
	if (result != FMOD_OK) 
	{
		printf("FMOD error: %d", result);
		releaseFMOD();
		system("pause");
		exit(1);
	}
}

bool initFMOD() 
{
	_result = FMOD::System_Create(&_system);
	errorCheck(_result);

	_result = _system->init(32, FMOD_INIT_NORMAL, 0);
	errorCheck(_result);

	return true;
}

void releaseFMOD() 
{
	std::vector<AudioItem>::iterator it = _audio_items.begin();
	for (it; it != _audio_items.end(); it++)
	{
		if (it->sound) {
			_result = it->sound->release();
			errorCheck(_result);
		}
	}

	if (_system) 
	{
		_result = _system->close();
		errorCheck(_result);
		_result = _system->release();
		errorCheck(_result);
	}
}

struct point 
{
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
};

#pragma region GLFW_HEADERS
bool init_gl();
bool init_text();
bool init_shaders();
void render_text(const char* text);
#pragma endregion

#pragma region GameLogic
bool initGame()
{
	_skeleton_index = 0;
	_player = Player();
	_boss = Boss("../common/assets/ascii/death.txt");

	_skeletons.clear();

	for (size_t i = 0; i < _skeleton_max; i++)
	{
		_skeletons.push_back(Skeleton("../common/assets/ascii/skeleton.txt"));
	}

	_game_lost = false;
	_game_won = false;

	for (AudioItem& ai : _audio_items)
	{
		ai.set_is_paused(true);
	}
	_audio_items[0].set_is_paused(false);

	return true;
}

void PlayerAttack()
{
	_audio_items[5].restart_sound();
	Sleep(500);
	if (_skeleton_index < _skeleton_max)
	{
		if (_skeletons[_skeleton_index].TakeDamage(_player.DealDamag()))
		{
			_audio_items[3].restart_sound();
			Sleep(500);
		}

		if (_skeletons[_skeleton_index].GetCurHP() <= 0)
			_skeleton_index++;
		if (_skeleton_index == _skeletons.size())
		{
			for (AudioItem& ai : _audio_items)
			{
				ai.set_is_paused(true);
			}
			_audio_items[1].restart_sound();
		}
	}
	else if (_boss.GetCurHP() > 0)
	{
		if (_boss.TakeDamage(_player.DealDamag()))
		{
			_audio_items[3].restart_sound();
			Sleep(500);
		}
	}

	if (_boss.GetCurHP() <= 0)
	{
		_game_won = true;
		for (AudioItem& ai : _audio_items)
		{
			ai.set_is_paused(true);
		}
		_audio_items[9].restart_sound();
	}
}

void EnemyAttack()
{
	if (_skeleton_index < _skeleton_max)
	{
		_audio_items[5].restart_sound();
		Sleep(1000);
		if (_player.TakeDamage(_skeletons[_skeleton_index].DealDamag()))
		{
			_audio_items[4].restart_sound();
			Sleep(1000);
		}
	}
	else if (_boss.GetCurHP() > 0)
	{
		_audio_items[5].restart_sound();
		Sleep(1000);
		if (_player.TakeDamage(_boss.DealDamag()))
		{
			_audio_items[4].restart_sound();
			Sleep(1000);
		}
	}

	if (_player.GetCurHP() <= 0)
	{
		_game_lost = true;
		for (AudioItem& ai : _audio_items)
		{
			ai.set_is_paused(true);
		}
		_audio_items[7].restart_sound();
		_audio_items[8].restart_sound();
	}
}

void Heal()
{
	_player.Heal();
	_audio_items[6].restart_sound();
	Sleep(1000);
}
#pragma endregion

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (mods == GLFW_MOD_SHIFT)
	{
		//Change Pitch
		if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
			_audio_items[_current_audio_item_index].change_pitch(0.01f);
		if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
			_audio_items[_current_audio_item_index].change_pitch(-0.01f);
		//Change Pan
		if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
			_audio_items[_current_audio_item_index].change_pan(0.05f);
		if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
			_audio_items[_current_audio_item_index].change_pan(-0.05f);

		//Restart Game
		if (key == GLFW_KEY_R && action == GLFW_PRESS)
		{
			initGame();
		}
	}
	else
	{
		//super basic game controls when gamestate is active
		if (!_game_lost && !_game_won && !_toggle_help)
		{
			if (key == GLFW_KEY_Z && action == GLFW_PRESS)
			{
				PlayerAttack();
				EnemyAttack();
			}

			if (key == GLFW_KEY_X && action == GLFW_PRESS)
			{
				Heal();
				EnemyAttack();
			}
		}

		// Toggle Help
		if (key == GLFW_KEY_H && action == GLFW_PRESS)
			_toggle_help = !_toggle_help;

		// Pause/Play
		if (key == GLFW_KEY_P && action == GLFW_PRESS)
			_audio_items[_current_audio_item_index].set_is_paused(!_audio_items[_current_audio_item_index].get_is_paused());

		// Next Track
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
			_current_audio_item_index = (_current_audio_item_index >= _audio_items.size() - 1) ? 0 : _current_audio_item_index + 1;
		// Previous Track
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
			_current_audio_item_index = (_current_audio_item_index == 0) ? _audio_items.size() - 1 : _current_audio_item_index - 1;

		//Reset Track
		if (key == GLFW_KEY_R && action == GLFW_PRESS)
			_audio_items[_current_audio_item_index].restart_sound();

		// Change Volume
		if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
			_audio_items[_current_audio_item_index].change_volume(0.01f);
		if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
			_audio_items[_current_audio_item_index].change_volume(-0.01f);

		//Select items in audio_items - should really check that items actually exist though....
		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
			_info_item = 1;
		if (key == GLFW_KEY_2 && action == GLFW_PRESS)
			_info_item = 2;
		if (key == GLFW_KEY_3 && action == GLFW_PRESS)
			_info_item = 3;
		if (key == GLFW_KEY_4 && action == GLFW_PRESS)
			_info_item = 4;
		if (key == GLFW_KEY_5 && action == GLFW_PRESS)
			_info_item = 5;
		if (key == GLFW_KEY_6 && action == GLFW_PRESS)
			_info_item = 6;
		if (key == GLFW_KEY_7 && action == GLFW_PRESS)
			_info_item = 7;
		if (key == GLFW_KEY_8 && action == GLFW_PRESS)
			_info_item = 8;
		if (key == GLFW_KEY_9 && action == GLFW_PRESS)
			_info_item = 9;
		if (key == GLFW_KEY_0 && action == GLFW_PRESS)
			_info_item = 0;
	}
}

int main() {
#pragma region Main_Initializers
	// FMOD STUFF
	fprintf(stdout, "Init FMOD...\n");
	assert(initFMOD());
	fprintf(stdout, "Init Audio Items...\n");
	assert(initAudioItems());

	fprintf(stdout, "Init opengl ...\n");
	assert(init_gl());
	fprintf(stdout, "Init text...\n");
	assert(init_text());
	fprintf(stdout, "Init shaders...\n");
	assert(init_shaders());

	assert(initGame());

	fprintf(stdout, "Ready ...!\n");
#pragma endregion

	//=======================================================================================
	//Main loop
	while (!glfwWindowShouldClose(_main_window)) {

		//Reset text y position
		_current_y_position = 30.0f;

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(_program);

		render_text("           Dylan Kirkby's Media Fundamentals P1");
		render_text("<=======================| Press H to Toggle Help");
		if (_toggle_help)
		{
			render_text("  ESC to Exit!");
			render_text("  'P' - Play/Pause Track");
			render_text("  'R' - reset track to 00:00,   +Shift to reset game state");
			render_text("  Left/Right - Change Sound,   +Shift for Pan");
			render_text("  Up/Down - Volume,   +Shift for Pitch.");
			render_text("<=======================>");
		}
		
		sprintf(_text_buffer, "Current Audio Item: %s", _audio_items[_current_audio_item_index].get_name().c_str());
		render_text(_text_buffer);

		switch (_info_item)
		{
		case 1:
			sprintf(_text_buffer, (_audio_items[_current_audio_item_index].get_is_paused()) ? "Paused: YES" : "Paused: NO");
			render_text(_text_buffer);
			break;
		case 2:
			sprintf(_text_buffer, "Channel - Volume: %d%% (0 - 100)", static_cast<int>(_audio_items[_current_audio_item_index].get_volume() * 100));
			render_text(_text_buffer);
			break;
		case 3:
			sprintf(_text_buffer, "Channel - Pitch: %.02f (0.5 - 2.0)", _audio_items[_current_audio_item_index].get_pitch());
			render_text(_text_buffer);
			break;
		case 4:
			sprintf(_text_buffer, "Channel - Balance: %.02f (-1.0 - 1.0)", _audio_items[_current_audio_item_index].get_pan());
			render_text(_text_buffer);
			break;
		case 5:
			sprintf(_text_buffer, "Channel - Time: %02d:%02d / %02d:%02d", _audio_items[_current_audio_item_index].get_position() / 1000 / 60,
				_audio_items[_current_audio_item_index].get_position() / 1000 % 60,
				_audio_items[_current_audio_item_index].get_length() / 1000 / 60,
				_audio_items[_current_audio_item_index].get_length() / 1000 % 60);
			render_text(_text_buffer);
			break;
		case 6:
			sprintf(_text_buffer, "Channel - Frequency: %f", _audio_items[_current_audio_item_index].get_frequency());
			render_text(_text_buffer);
			break;
		case 7:
			sprintf(_text_buffer, "Format: %s", _audio_items[_current_audio_item_index].get_format().c_str());
			render_text(_text_buffer);
			break;
		case 8:
			sprintf(_text_buffer, "Type: %s", _audio_items[_current_audio_item_index].get_type().c_str());
			render_text(_text_buffer);
			break;
		case 9:
			sprintf(_text_buffer, "Sound - Number of Channels: %d", _audio_items[_current_audio_item_index].get_channels());
			render_text(_text_buffer);
			break;
		case 0:
			sprintf(_text_buffer, "Sound - Number of bits: %d", _audio_items[_current_audio_item_index].get_bits());
			render_text(_text_buffer);
			break;
		default:
			render_text("Use number keys to check currently selected sound's information.");
			break;
		}

		render_text("<=======================| Game");
		if (!_toggle_help && !_game_won && !_game_lost)
		{
			if (_skeleton_index < _skeleton_max)
			{
				for (size_t i = 0; i < _skeletons[_skeleton_index].asciiArt.size(); i++)
				{
					render_text(_skeletons[_skeleton_index].asciiArt[i].c_str());
				}
				render_text("<=======================>");
				sprintf(_text_buffer, "<==| Z: Attack    <==| Skeleton %d HP: %d / %d",
					_skeleton_index + 1,
					_skeletons[_skeleton_index].GetCurHP(),
					_skeletons[_skeleton_index].GetMaxHP());
				render_text(_text_buffer);
			}
			else if (_boss.GetCurHP() > 0)
			{
				for (size_t i = 0; i < _boss.asciiArt.size(); i++)
				{
					render_text(_boss.asciiArt[i].c_str());
				}
				render_text("<=======================>");
				sprintf(_text_buffer, "<==| Z: Attack    <==| BOSS HP: %d / %d",
					_boss.GetCurHP(),
					_boss.GetMaxHP());
				render_text(_text_buffer);

			}
			sprintf(_text_buffer, "<==| X: Heal      <==| Your HP: %d / %d",
				_player.GetCurHP(),
				_player.GetMaxHP());
			render_text(_text_buffer);
		}
		else if (_game_won)
		{
			render_text("");
			render_text("");
			render_text("     YOU WIN    ");
		}
		else if (_game_lost)
		{
			render_text("     GAME OVER    ");
			render_text("                         /  `Y`");
			render_text("                    _   /   /");
			render_text("                   {_'-K.__/");
			render_text("                     `/-.__L._");
			render_text("                     /  ' /`\\_}");
			render_text("                    /  ' /");
			render_text("            ____   /  ' /");
			render_text("     ,-'~~~~    ~~/  ' /_");
			render_text("   ,'             ``~~~%%',");
			render_text("  (                     %  Y");
			render_text(" {                      %% I");
			render_text("{      -                 %  `.");
			render_text("|       ',                %  )");
			render_text("|        |   ,..__      __. Y");
			render_text("|    .,_./  Y ' / ^Y   J   )|");
			render_text("\\           |' /   |   |   ||");
			render_text(" \\          L_/    . _ (_,.'(");
			render_text("  \\,   ,      ^^""' / |      )");
		}

		glfwSwapBuffers(_main_window);
		glfwPollEvents();

		glfwGetWindowSize(_main_window, &_window_width, &_window_height);
	}

	releaseFMOD();

	glfwDestroyWindow(_main_window);
	glfwTerminate();

	return 0;
}

#pragma region GL_Functions
bool init_gl() {

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "Unable to init glfw\n");
		return false;
	}

	//Full screen
	//_main_window = glfwCreateWindow(1920, 1080, "Media Fundamentals... play sound", glfwGetPrimaryMonitor(), NULL);
	_main_window = glfwCreateWindow(1920, 1080, "Media Fundamentals... play sound", NULL, NULL);

	if (!_main_window)
	{
		glfwTerminate();
		fprintf(stderr, "Unable to create main window glfw\n");
		return false;
	}

	glfwGetWindowSize(_main_window, &_window_width, &_window_height);

	glfwSetKeyCallback(_main_window, key_callback);
	glfwMakeContextCurrent(_main_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Unable to init glad\n");
		return false;
	}
	glfwSwapInterval(1);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClearColor(0.3f, 0.1f, 0.1f, 1.0f);

	return true;
}

bool init_text() {

	if (FT_Init_FreeType(&_ft)) {
		fprintf(stderr, "Unable to init text...\n");
		return false;
	}
	if (FT_New_Face(_ft, "../common/assets/fonts/arial.ttf", 0, &_face)) {
		fprintf(stderr, "Unable to init text...\n");
		return false;
	}

	//Font size
	FT_Set_Pixel_Sizes(_face, 0, 20);

	if (FT_Load_Char(_face, 'X', FT_LOAD_RENDER))
	{
		fprintf(stderr, "unable to load character\n");
		return false;
	}

	return true;
}

bool init_shaders() {
	//=======================================================================================
//Shaders
	int params = -1;

	glGetProgramiv(_program, GL_LINK_STATUS, &params);
	if (!params) {
		fprintf(stderr, "Unable to link shader program");
		return 1;
	}
	//--	
	FILE* f = fopen("../common/src/shaders/simple_text.vert", "rb");
	long filesize = ftell(f);
	fseek(f, 0, SEEK_END);
	filesize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* simple_text_vert = (char*)malloc(filesize + 1);
	fread(simple_text_vert, filesize, 1, f);
	fclose(f);
	simple_text_vert[filesize] = 0;
	//--
	f = fopen("../common/src/shaders/simple_text.frag", "rb");
	fseek(f, 0, SEEK_END);
	filesize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* simple_text_frag = (char*)malloc(filesize + 1);
	fread(simple_text_frag, filesize, 1, f);
	fclose(f);
	simple_text_frag[filesize] = 0;

	//text vertex shader
	unsigned int simple_text_vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(simple_text_vs, 1, &simple_text_vert, NULL);
	glCompileShader(simple_text_vs);
	glGetShaderiv(simple_text_vs, GL_COMPILE_STATUS, &params);
	if (!params) {
		fprintf(stderr, "Unable to compile simple text vertex shader");
		return 1;
	}
	unsigned int simple_text_fg = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(simple_text_fg, 1, &simple_text_frag, NULL);
	glCompileShader(simple_text_fg);
	glGetShaderiv(simple_text_fg, GL_COMPILE_STATUS, &params);
	if (!params) {
		fprintf(stderr, "Unable to compile simple text fragment shader");
		return 1;
	}

	_text_program = glCreateProgram();
	glAttachShader(_text_program, simple_text_vs);
	glAttachShader(_text_program, simple_text_fg);
	glLinkProgram(_text_program);

	glGetProgramiv(_text_program, GL_LINK_STATUS, &params);
	if (!params) {
		fprintf(stderr, "Unable to link simple text shader program");
		return 1;
	}

	_uniform_tex = glGetUniformLocation(_text_program, "tex");
	_attribute_coord = glGetAttribLocation(_text_program, "coord");
	_uniform_color = glGetUniformLocation(_text_program, "color");
	glGenBuffers(1, &_dp_vbo);

	glUseProgram(_program);
	return true;
}

void render_text(const char* text) {
	//render_text(const char *text, float x, float y, float sx, float sy)
	float sx = 2.0f / _window_width;
	float sy = 2.0f / _window_height;

	//GLfloat _current_y_position = 30.0f;
	GLfloat xoffset = 8 * sx;
	float x = -1 + xoffset;
	float y = 1 - _current_y_position * sy;


	glUseProgram(_text_program);
	GLfloat cyan[4] = { 0, 1, 1, 1 };
	glUniform4fv(_uniform_color, 1, cyan);
	const char* p;
	FT_GlyphSlot g = _face->glyph;

	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(_uniform_tex, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Set up the VBO for our vertex data */
	glEnableVertexAttribArray(_attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, _dp_vbo);
	glVertexAttribPointer(_attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);


	/* Loop through all characters */
	for (p = text; *p; p++) {
		/* Try to load and render the character */
		if (FT_Load_Char(_face, *p, FT_LOAD_RENDER))
			continue;


		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			g->bitmap.width,
			g->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			g->bitmap.buffer
		);

		/* Calculate the vertex and texture coordinates */
		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		point box[4] = {
			{ x2, -y2, 0, 0 },
		{ x2 + w, -y2, 1, 0 },
		{ x2, -y2 - h, 0, 1 },
		{ x2 + w, -y2 - h, 1, 1 },
		};


		/* Draw the character on the screen */
		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		/* Advance the cursor to the start of the next character */
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}
	glDisableVertexAttribArray(_attribute_coord);
	glDeleteTextures(1, &tex);
	glEnableVertexAttribArray(0);

	_current_y_position = (_current_y_position > 800.0f) ? 30.0f : (_current_y_position + _y_offset);
}
#pragma endregion