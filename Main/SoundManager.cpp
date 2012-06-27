/*
 * SoundManager.cpp
 *
 *  Created on: Jun 13, 2011
 *      Author: hendrik
 */

#include "SoundManager.h"
#include "FlyingObject.h"

#include <time.h>

using namespace std;

SoundManager::SoundManager() {
	audio_rate = 22050;
	audio_format = AUDIO_S16; /* 16-bit stereo */
	audio_channels = 2;
	audio_buffers = 4096;

	SDL_Init(SDL_INIT_AUDIO);

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
	    printf("Unable to open audio!\n");
	    exit(1);
	}

	// INIT SOUNDS
	m_hitChunk = Mix_LoadWAV("../SoundPack/Left_Hook.wav");
	m_electroChunk = Mix_LoadWAV("../SoundPack/Electricity.wav");
	m_rotateChunk = Mix_LoadWAV("../SoundPack/Old_Door.wav");
	m_gameStartChunk = Mix_LoadWAV("../SoundPack/Electrical_Sweep.wav");

	Mix_VolumeMusic(20);
	music = Mix_LoadMUS("../SoundPack/HoldinBack.wav");

	Mix_VolumeMusic(128);

	music_easy1 = Mix_LoadMUS("../SoundPack/DifficultyThemes/Easy/ComeBackToMe.wav");
	music_easy2 = Mix_LoadMUS("../SoundPack/DifficultyThemes/Easy/DangerInTheClouds.wav");
	music_easy3 = Mix_LoadMUS("../SoundPack/DifficultyThemes/Easy/RedSky.wav");
	music_easy4 = Mix_LoadMUS("../SoundPack/DifficultyThemes/Easy/WaveChasers.wav");

	music_medium1 = Mix_LoadMUS("../SoundPack/DifficultyThemes/Medium/Answers.wav");
	music_medium2 = Mix_LoadMUS("../SoundPack/DifficultyThemes/Medium/DaydreamBeliever.wav");
	music_medium3 = Mix_LoadMUS("../SoundPack/DifficultyThemes/Medium/Imitator.wav");
	music_medium4 = Mix_LoadMUS("../SoundPack/DifficultyThemes/Medium/SorryNoMore.wav");

	music_hard1 = Mix_LoadMUS("../SoundPack/DifficultyThemes/Hard/AllForYou.wav");
	music_hard2 = Mix_LoadMUS("../SoundPack/DifficultyThemes/Hard/Anatak.wav");
	music_hard3 = Mix_LoadMUS("../SoundPack/DifficultyThemes/Hard/SurfNation.wav");
	music_hard4 = Mix_LoadMUS("../SoundPack/DifficultyThemes/Hard/TwistingTheWorld.wav");

	Mix_VolumeMusic(20);
	playMenuTheme();

	initMap();
	setTrigger(Message::MSG_COL_PLAYER_LETTER);
	setTrigger(Message::MSG_COL_PLAYER_FLY);
	setTrigger(Message::MSG_NEXT_LEVEL);
	setTrigger(Message::MSG_GAME_GO);
	setTrigger(Message::MSG_WON);
	setTrigger(Message::MSG_END_GAME);
	setTrigger(Message::MSG_END_GAME_WITHOUT_HS);

}

void SoundManager::initMap(){
	messageMap[">"] = SEL_ROTATE;
	messageMap["<"] = SEL_ROTATE;
	messageMap["PLAY GAME"] = SEL_LETTERS;
	messageMap["EXIT"] = SEL_LETTERS;
	messageMap["HIGHSCORES"] = SEL_LETTERS;
	messageMap["BACK"] = SEL_LETTERS;
	messageMap["ARCADE"] = SEL_LETTERS;
	messageMap["EASY"] = SEL_GAME_START;
	messageMap["MEDIUM"] = SEL_GAME_START;
	messageMap["HARD"] = SEL_GAME_START;
	messageMap["MADNESS"] = SEL_GAME_START;
	messageMap["ARCADE"] = SEL_LETTERS;
	messageMap["SPARTA"] = SEL_LETTERS;
	messageMap["HIT TRAINING"] = SEL_LETTERS;
	messageMap["ENTER NAME"] = SEL_LETTERS;
	messageMap["A"] = SEL_LETTERS;messageMap["B"] = SEL_LETTERS;messageMap["C"] = SEL_LETTERS;
	messageMap["D"] = SEL_LETTERS;messageMap["E"] = SEL_LETTERS;messageMap["F"] = SEL_LETTERS;
	messageMap["G"] = SEL_LETTERS;messageMap["H"] = SEL_LETTERS;messageMap["I"] = SEL_LETTERS;
	messageMap["J"] = SEL_LETTERS;messageMap["K"] = SEL_LETTERS;messageMap["L"] = SEL_LETTERS;
	messageMap["M"] = SEL_LETTERS;messageMap["N"] = SEL_LETTERS;messageMap["O"] = SEL_LETTERS;
	messageMap["P"] = SEL_LETTERS;messageMap["Q"] = SEL_LETTERS;messageMap["R"] = SEL_LETTERS;
	messageMap["S"] = SEL_LETTERS;messageMap["T"] = SEL_LETTERS;messageMap["U"] = SEL_LETTERS;
	messageMap["V"] = SEL_LETTERS;messageMap["W"] = SEL_LETTERS;messageMap["X"] = SEL_LETTERS;
	messageMap["Y"] = SEL_LETTERS;messageMap["Z"] = SEL_LETTERS;
	messageMap["GOAL REACHED"] = SEL_WON;
}

void SoundManager::trigger(Message msg){
	string type = msg.menuPointWord;
	if(msg.messageType == Message::MSG_COL_PLAYER_FLY){
		if(msg.additionalInfo == FlyingObject::FOM_BAD){
			Mix_PlayChannel(2,m_electroChunk,0);
		}
		if(msg.additionalInfo == FlyingObject::FOM_REFLECT){
			Mix_PlayChannel(2,m_hitChunk,0);
		}
	}
	else if(msg.messageType == Message::MSG_NEXT_LEVEL){
		Mix_PlayChannel(-1,m_gameStartChunk,0);
	}
	else if(msg.messageType == Message::MSG_END_GAME || msg.messageType == Message::MSG_END_GAME_WITHOUT_HS){
		Mix_FadeOutMusic(1000);
	}
	else if(msg.messageType == Message::MSG_GAME_GO){
		playIngameTheme();
	}
	else if(msg.messageType == Message::MSG_WON){
		Mix_FadeOutMusic(1000);
	}
	else if(messageMap.find(type)!=messageMap.end()){
		int mesType = messageMap.at(type);

		switch(mesType){

		case SEL_ROTATE 	: //Mix_PlayChannel(-1,m_hitChunk,0);
							  Mix_PlayChannel(2,m_rotateChunk,0);
							  break;
		case SEL_LETTERS 	: Mix_PlayChannel(-1,m_hitChunk,0);
							  break;
		case SEL_GAME_START : Mix_PlayChannel(-1,m_hitChunk,0);
							  Mix_HaltMusic();
							  Mix_PlayChannel(-1,m_gameStartChunk,0);
							  break;
		case SEL_WON 		: Mix_PlayMusic(music,-1);
							  break;

		default          	: break;
		}
	}
}

SoundManager::~SoundManager() {
	Mix_CloseAudio();
	SDL_Quit();

}

void SoundManager::playMenuTheme(){
	Mix_PlayMusic(music,-1);
}

void SoundManager::playIngameTheme(){
	Mix_VolumeMusic(128);
	int random = rand() % 4;
//	printf("the random number is before: %i\n",random);
	switch(m_difficulty){
	case ArcadeConfiguration::DIFF_EASY :
		switch(random){
		case 0 : Mix_PlayMusic(music_easy1,-1); break;
		case 1 : Mix_PlayMusic(music_easy2,-1); break;
		case 2 : Mix_PlayMusic(music_easy3,-1); break;
		case 3 : Mix_PlayMusic(music_easy4,-1); break;
		}
		break;
	case ArcadeConfiguration::DIFF_MEDIUM :
		switch(random){
		case 0 : Mix_PlayMusic(music_medium1,-1); break;
		case 1 : Mix_PlayMusic(music_medium2,-1); break;
		case 2 : Mix_PlayMusic(music_medium3,-1); break;
		case 3 : Mix_PlayMusic(music_medium4,-1); break;
		}
		break;
	case ArcadeConfiguration::DIFF_HARD :
		switch(random){
		case 0 : Mix_PlayMusic(music_hard1,-1); break;
		case 1 : Mix_PlayMusic(music_hard2,-1); break;
		case 2 : Mix_PlayMusic(music_hard3,-1); break;
		case 3 : Mix_PlayMusic(music_hard4,-1); break;
		}
		break;
	case ArcadeConfiguration::DIFF_MADNESS :
		music_madness = Mix_LoadMUS("../SoundPack/DifficultyThemes/Madness/MortalKombat.wav");
		Mix_PlayMusic(music_madness,-1);
		break;
	}
}

void SoundManager::setDifficulty(int diff){
	m_difficulty = diff;
}
