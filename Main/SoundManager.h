/*
 * SoundManager.h
 *
 *  Created on: Jun 13, 2011
 *      Author: hendrik
 */

#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_mixer.h"
#include "Message.h"
#include "SignalUser.h"

#include "GameConfigurations/ArcadeConfiguration.h"

class SoundManager : public SignalUser {
public:
	SoundManager();
	virtual ~SoundManager();

	enum selectionType{
			SEL_ROTATE = 0,
			SEL_LETTERS,
			SEL_GAME_START,
			SEL_WON,
			NUM_TYPES
	};

private:
	Mix_Music* music;
	Mix_Music* music_easy1;
	Mix_Music* music_easy2;
	Mix_Music* music_easy3;
	Mix_Music* music_easy4;
	Mix_Music* music_medium1;
	Mix_Music* music_medium2;
	Mix_Music* music_medium3;
	Mix_Music* music_medium4;
	Mix_Music* music_hard1;
	Mix_Music* music_hard2;
	Mix_Music* music_hard3;
	Mix_Music* music_hard4;
	Mix_Music* music_madness;
	Mix_Chunk* m_hitChunk;
	Mix_Chunk* m_electroChunk;
	Mix_Chunk* m_rotateChunk;
	Mix_Chunk* m_gameStartChunk;


	std::map<std::string,selectionType> messageMap;

	int audio_rate;
	Uint16 audio_format;
	int audio_channels;
	int audio_buffers;

public:
	void trigger(Message msg);
	void setDifficulty(int diff);

private:
	void initMap();
	void playMenuTheme();
	void playIngameTheme();

	int m_difficulty;

};

#endif /* SOUNDMANAGER_H_ */
