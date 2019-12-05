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

#include "cAudioItem.hpp"
#include "cAudioManager.hpp"

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
#pragma endregion

#define BUFFER_SIZE 255

//global audio item objects
std::map<std::string, FMOD::ChannelGroup*> mpChannelGoups;
std::map<std::string, cAudioItem*> mpAudio;
std::vector<DSP> g_vec_DSP;
//std::vector<AudioItem> _audio_items;
int _current_audio_item_index = 0;

#define LATENCY_MS      (50) /* Some devices will require higher latency to avoid glitches */
#define DRIFT_MS        (1)
#define DEVICE_INDEX    (0)
FMOD::Sound* _sound = NULL;
FMOD::Channel* _channel = NULL;
bool _record = false;
cAudioManager* pAudioManager = cAudioManager::GetAudioManager();

//TODO - MOVE INTO AUDIO MANAGER
//bool initAudioItems()
//{
//	rapidxml::xml_document<> document;
//	rapidxml::xml_node<>* root_node;
//
//	char input;
//	bool usercontrol = true;
//	std::string path;
//	do
//	{
//		printf("Load Compressed Sounds (Y/N): ");
//		std::cin >> input;
//		if (input == 'y' || input == 'Y')
//		{
//			usercontrol = false;
//			path = "./config_files/compressed_audio_file_paths.xml";
//		}
//		else if (input == 'n' || input == 'N')
//		{
//			usercontrol = false;
//			path = "./config_files/uncompressed_audio_file_paths.xml";
//		}
//		else
//			printf("Please select an option...\n");
//	} while (usercontrol);
//
//	// Read XML file
//	std::ifstream soundFiles(path);
//
//	if (!soundFiles)
//	{
//		printf("Unable to open %s\n", path.c_str());
//		system("pause");
//		return false;
//	}
//
//	// Copy the file contents into our buffer
//	std::vector<char> buffer((std::istreambuf_iterator<char>(soundFiles)), std::istreambuf_iterator<char>());
//	buffer.push_back('\0');
//
//	// Parse the buffer using the xml parsinglib
//	document.parse<0>(&buffer[0]);
//
//	// Find the root node
//	root_node = document.first_node("SoundFiles");
//
//	// Iterate through all of the root node's children
//	for (rapidxml::xml_node<>* platform_node = root_node->first_node("File");
//		platform_node;
//		platform_node = platform_node->next_sibling())
//	{
//		int i = 0;
//		if (platform_node->first_attribute("path")->value() > 0)
//		{
//			/*AudioItem ai;
//			ai.set_path(platform_node->first_attribute("path")->value());
//			if (*platform_node->last_attribute("stream")->value() == 't')
//				ai.create_and_play_sound(true, i++);
//			else
//				ai.create_and_play_sound(false, i++);
//			_audio_items.push_back(ai);*/
//		}
//	}
//
//	return true;
//}

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

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

// TODO - move to its own function, add controls
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		bool isRecording = false;
		pAudioManager->_system->isRecording(DEVICE_INDEX, &isRecording);

		if (isRecording)
		{
			pAudioManager->_system->recordStop(DEVICE_INDEX);
			_channel->stop();
		}
		else
		{
			/* Clean up previous record sound */
			if (_sound)
			{
				pAudioManager->_result = _sound->release();
				pAudioManager->error_check();
			}

			/* Query device native settings and start a recording */
			int nativeRate = 0;
			int nativeChannels = 0;
			pAudioManager->_result = pAudioManager->_system->getRecordDriverInfo(DEVICE_INDEX, NULL, 0, NULL, &nativeRate, NULL, &nativeChannels, NULL);
			pAudioManager->error_check();

			FMOD_CREATESOUNDEXINFO exinfo = { 0 };
			exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
			exinfo.numchannels = nativeChannels;
			exinfo.format = FMOD_SOUND_FORMAT_PCM16;
			exinfo.defaultfrequency = nativeRate;
			exinfo.length = nativeRate * sizeof(short) * nativeChannels; /* 1 second buffer, size here doesn't change latency */

			pAudioManager->_result = pAudioManager->_system->createSound(0, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &_sound);
			pAudioManager->error_check();

			pAudioManager->_result = pAudioManager->_system->recordStart(DEVICE_INDEX, _sound, true);
			if (pAudioManager->_result != FMOD_ERR_RECORD_DISCONNECTED)
			{
				pAudioManager->error_check();
			}

			if (_sound)
			{
				pAudioManager->_result = pAudioManager->_system->playSound(_sound, NULL, false, &_channel);
				pAudioManager->error_check();
			}
		}

	}

	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		bool bypass;

		pAudioManager->_result = g_vec_DSP[0].dsp->getBypass(&bypass);
		pAudioManager->error_check();

		bypass = !bypass;

		pAudioManager->_result = g_vec_DSP[0].dsp->setBypass(bypass);
		pAudioManager->error_check();
	}

	if (key == GLFW_KEY_6 && action == GLFW_PRESS)
	{
		bool bypass;

		pAudioManager->_result = g_vec_DSP[1].dsp->getBypass(&bypass);
		pAudioManager->error_check();

		bypass = !bypass;

		pAudioManager->_result = g_vec_DSP[1].dsp->setBypass(bypass);
		pAudioManager->error_check();
	}

	if (key == GLFW_KEY_7 && action == GLFW_PRESS)
	{
		bool bypass;

		pAudioManager->_result = g_vec_DSP[2].dsp->getBypass(&bypass);
		pAudioManager->error_check();

		bypass = !bypass;

		pAudioManager->_result = g_vec_DSP[2].dsp->setBypass(bypass);
		pAudioManager->error_check();
	}

	if (mods == GLFW_MOD_SHIFT) {
		
	}
	else {
		
	}
}

int main() {
#pragma region Main_Initializers
	// FMOD STUFF
	pAudioManager->error_check();

	//pAudioManager->LoadDSPs();
	//pAudioManager->error_check();
	//
	//assert(pAudioManager->LoadNetSounds());
	//pAudioManager->error_check();
	//
	//assert(pAudioManager->LoadTextToSpeech());
	//pAudioManager->error_check();

	//assert(pAudioManager->LoadTextToWav());
	//pAudioManager->error_check();

	fprintf(stdout, "Init opengl...\n");
	assert(init_gl());
	fprintf(stdout, "Init text...\n");
	assert(init_text());
	fprintf(stdout, "Init shaders...\n");
	assert(init_shaders());

	fprintf(stdout, "Ready ...!\n");
#pragma endregion

#pragma region Checks
	unsigned int version = 0;
	pAudioManager->_result = pAudioManager->_system->getVersion(&version);
	pAudioManager->error_check();

	if (version < FMOD_VERSION)
	{
		error_callback(0, "FMOD lib version doesn't match header version");
		pAudioManager->Release();
		system("pause");
		exit(1);
	}

	int numDrivers = 0;
	pAudioManager->_result = pAudioManager->_system->getRecordNumDrivers(NULL, &numDrivers);
	pAudioManager->error_check();

	if (numDrivers == 0)
	{
		error_callback(0, "No recording devices found/plugged in!  Aborting.");
		pAudioManager->Release();
		system("pause");
		exit(1);
	}
#pragma endregion

#pragma region Record PlaybackRate
	unsigned int samplesRecorded = 0;
	unsigned int samplesPlayed = 0;

	/*Determine latency in samples.*/
	int nativeRate = 0;
	int nativeChannels = 0;
	pAudioManager->_result = pAudioManager->_system->getRecordDriverInfo(DEVICE_INDEX, NULL, 0, NULL, &nativeRate, NULL, &nativeChannels, NULL);
	pAudioManager->error_check();

	unsigned int driftThreshold = (nativeRate * DRIFT_MS) / 1000;       /* The point where we start compensating for drift */
	unsigned int desiredLatency = (nativeRate * LATENCY_MS) / 1000;     /* User specified latency */
	unsigned int adjustedLatency = desiredLatency;                      /* User specified latency adjusted for driver update granularity */
	int actualLatency = desiredLatency;                                 /* Latency measured once playback begins (smoothened for jitter) */

	unsigned int soundLength = 0;
#pragma endregion

#pragma region RecordDSPs
	pAudioManager->LoadDSPs();

	FMOD::ChannelGroup* mastergroup = 0;
	pAudioManager->_result = pAudioManager->_system->getMasterChannelGroup(&mastergroup);
	pAudioManager->error_check();

	pAudioManager->_result = mastergroup->addDSP(0, g_vec_DSP[0].dsp);
	pAudioManager->error_check();
	pAudioManager->_result = g_vec_DSP[0].dsp->setBypass(true);
	pAudioManager->error_check();
	pAudioManager->_result = mastergroup->addDSP(0, g_vec_DSP[1].dsp);
	pAudioManager->error_check();
	pAudioManager->_result = g_vec_DSP[1].dsp->setBypass(true);
	pAudioManager->error_check();
	pAudioManager->_result = mastergroup->addDSP(0, g_vec_DSP[2].dsp);
	pAudioManager->error_check();
	pAudioManager->_result = g_vec_DSP[2].dsp->setBypass(true);
	pAudioManager->error_check();
#pragma endregion

	//=======================================================================================
	//Main loop
	while (!glfwWindowShouldClose(_main_window)) {

		/*******************************RecordPlaybackRate*******************************/
		bool isRecording = false;
		pAudioManager->_result = pAudioManager->_system->isRecording(DEVICE_INDEX, &isRecording);
		pAudioManager->error_check();

		if (isRecording)
		{
			pAudioManager->_result = _sound->getLength(&soundLength, FMOD_TIMEUNIT_PCM);
			pAudioManager->error_check();

			/*Determine how much has been recorded since we last checked*/
			unsigned int recordPos = 0;
			pAudioManager->_result = pAudioManager->_system->getRecordPosition(DEVICE_INDEX, &recordPos);
			if (pAudioManager->_result != FMOD_ERR_RECORD_DISCONNECTED)
			{
				pAudioManager->error_check();
			}

			static unsigned int lastRecordPos = 0;
			unsigned int recordDelta = (recordPos >= lastRecordPos) ? (recordPos - lastRecordPos) : (recordPos + soundLength - lastRecordPos);
			lastRecordPos = recordPos;
			samplesRecorded += recordDelta;

			static unsigned int minRecordDelta = (unsigned int)-1;
			if (recordDelta && (recordDelta < minRecordDelta))
			{
				minRecordDelta = recordDelta; /* Smallest driver granularity seen so far */
				adjustedLatency = (recordDelta <= desiredLatency) ? desiredLatency : recordDelta; /* Adjust our latency if driver granularity is high */
			}

			/*Determine how much has been played since we last checked.*/
			unsigned int playPos = 0;
			pAudioManager->_result = _channel->getPosition(&playPos, FMOD_TIMEUNIT_PCM);
			pAudioManager->error_check();

			static unsigned int lastPlayPos = 0;
			unsigned int playDelta = (playPos >= lastPlayPos) ? (playPos - lastPlayPos) : (playPos + soundLength - lastPlayPos);
			lastPlayPos = playPos;
			samplesPlayed += playDelta;

			/*Compensate for any drift.*/
			int latency = samplesRecorded - samplesPlayed;
			actualLatency = (int)((0.97f * actualLatency) + (0.03f * latency));

			int playbackRate = nativeRate;
			if (actualLatency < (int)(adjustedLatency - driftThreshold))
			{
				/* Play position is catching up to the record position, slow playback down by 2% */
				playbackRate = nativeRate - (nativeRate / 50);
			}
			else if (actualLatency > (int)(adjustedLatency + driftThreshold))
			{
				/* Play position is falling behind the record position, speed playback up by 2% */
				playbackRate = nativeRate + (nativeRate / 50);
			}

			pAudioManager->_result = _channel->setFrequency((float)playbackRate);
			pAudioManager->error_check();
		}

		/*******************************RecordPlaybackRate*******************************/

		//Reset text y position
		_current_y_position = 30.0f;

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(_program);

		render_text("           Media Fundamentals P3");
		render_text("<=======================>");
		render_text("  ESC to Exit!");
		render_text("<=======================>");

		//sprintf(_text_buffer, "Current Audio Item: %s", _audio_items[_current_audio_item_index].get_name().c_str());
		//render_text(_text_buffer);

		render_text("");

		sprintf(_text_buffer, "Press 4 to %s", isRecording ? "start recording" : "stop recording");
		render_text(_text_buffer);

		bool bypass;
		g_vec_DSP[0].dsp->getBypass(&bypass);
		sprintf(_text_buffer, "Press 5 to %s REVERB DSP effect", !bypass ? "disable" : "enable");
		render_text(_text_buffer);
		g_vec_DSP[1].dsp->getBypass(&bypass);
		sprintf(_text_buffer, "Press 6 to %s HIGHPASS DSP effect", !bypass ? "disable" : "enable");
		render_text(_text_buffer);
		g_vec_DSP[2].dsp->getBypass(&bypass);
		sprintf(_text_buffer, "Press 7 to %s LOWPASS DSP effect", !bypass ? "disable" : "enable");
		render_text(_text_buffer);

		glfwSwapBuffers(_main_window);
		glfwPollEvents();

		glfwGetWindowSize(_main_window, &_window_width, &_window_height);
	}

	pAudioManager->Release();

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