/*
 * LevelHandler.cpp
 *
 *  Created on: Jul 4, 2011
 *      Author: hendrik
 */

#include "LevelHandler.h"
#include "FlyingObject.h"
#include <ostream>
#include <sstream>
#include <iostream>
#include <stdio.h>

LevelHandler* LevelHandler::theInstance = 0;

using namespace std;


LevelHandler::LevelHandler() {
	m_currentLevel = 0;
	maxLevels = 0;
	m_badHits = 0;
	m_goodHits = 0;
	m_reflectHits = 0;
	m_notHit = 0;
	m_started = false;

	m_timeRemaining = -1;
	m_ballsRemaining = -1;
	m_points = -1;
	m_livesRemaining = -1;

	m_ballThresh = 0;
	m_lifeThresh = 0;
	m_pointThresh = 0;
	m_timeThresh = 0;

	m_pointingSystem = PointingSystem::getInstance();

	setTrigger(Message::MSG_COL_PLAYER_FLY);
	setTrigger(Message::MSG_START_GAME);
	setTrigger(Message::MSG_RETURN_GAME);
	setTrigger(Message::MSG_PAUSE_GAME);
	setTrigger(Message::MSG_COL_FLYING_BOUNDARY);
	setTrigger(Message::MSG_SHOT_OBJECT);

	m_timePaused = 0;

	m_clock = new btClock();
	m_pauseClock = new btClock();

	m_ballWeight = 7;
	m_liveWeight = 10;
	m_timeWeight = 5;

	m_bonusBalls = 0;
	m_bonusLifes = 0;
	m_bonusTime = 0;


}

void LevelHandler::initialize(){
	m_badHits = 0;
	m_goodHits = 0;
	m_reflectHits = 0;
	m_notHit = 0;
	m_bonusBalls = 0;
	m_bonusLifes = 0;
	m_bonusTime = 0;
}

int LevelHandler::getBonusTime(){
	return m_bonusTime;
}
int LevelHandler::getBonusLifes(){
	return m_bonusLifes;
}
int LevelHandler::getBonusBalls(){
	return m_bonusBalls;
}

LevelHandler* LevelHandler::getInstance(){
	if(theInstance == 0){
		theInstance = new LevelHandler();
	}

	return theInstance;
}

void LevelHandler::destroy(){
	if(theInstance != 0){
		delete theInstance;
	}

	theInstance = 0;
}

LevelHandler::~LevelHandler() {
	delete m_clock;
}

int LevelHandler::getCurrentLevel() const
{
    return m_currentLevel;
}

void LevelHandler::setCurrentLevel(int currentLevel)
{
	m_started = true;
	m_clock->reset();
	ArcadeLevel* arcadeLevel;
	SpartaLevel* spartaLevel;
	switch (m_mode) {
		case ArcadeConfiguration::MODE_ARCADE:
			arcadeLevel = m_arcadeConfig->getLevel(currentLevel);
			m_ballThresh = arcadeLevel->getBallThresh();
			m_lifeThresh = arcadeLevel->getLiveThresh();
			m_pointThresh = arcadeLevel->getPointThresh();
			m_timeThresh = arcadeLevel->getTimeThresh();
			m_notHit = 0;
			break;
		case ArcadeConfiguration::MODE_SPARTA:
			m_ballThresh = 0;
			m_lifeThresh = 0;
			m_pointThresh = 0;
			m_timeThresh = 0;
			spartaLevel = m_spartaConfig->getLevel(currentLevel);
			m_ballThresh = spartaLevel->getBallThresh();
			m_notHit = 0;
			break;
		default:
			break;
	}

	m_descriptionString.clear();

	ostringstream ss;

	if(m_pointThresh > 0){
		ss << "Collect " << m_pointThresh << " points";
	} else {
		ss << "Collect as many points as you can";
	}
	m_descriptionString.push_back(ss.str());
	ss.str("");

	if(m_ballThresh > 0){
		ss << "with " << m_ballThresh << " Flying Objects";
	} else if(m_timeThresh > 0){
		ss << "within " << m_timeThresh << " seconds";
	}

	m_descriptionString.push_back(ss.str());
	ss.str("");

	if(m_lifeThresh > 0){
		ss <<  "given " << m_lifeThresh << " lives. Red Objects hurt you";
	}
	m_descriptionString.push_back(ss.str());
	ss.str("");

	if(m_lifeThresh > 0 && m_ballThresh > 0 && m_pointThresh == 0 && m_timeThresh == 0){
		m_descriptionString.clear();
		ostringstream sb;
		sb << "Survive " << m_ballThresh << " red Objects.";
		m_descriptionString.push_back(sb.str());
		sb.str("");
		sb << "You have " << m_lifeThresh << " lives.";
		m_descriptionString.push_back(sb.str());

	} else {

	}

    this->m_currentLevel = currentLevel;
}

int LevelHandler::getMaxLevels() const
{
    return maxLevels;
}

void LevelHandler::setMaxLevels(int maxLevels)
{
    this->maxLevels = maxLevels;
}

void LevelHandler::setArcadeConfiguration(ArcadeConfiguration* config){
	m_mode = ArcadeConfiguration::MODE_ARCADE;
	m_arcadeConfig = config;
}

void LevelHandler::setSpartaConfiguration(SpartaConfiguration* config){
	m_mode = ArcadeConfiguration::MODE_SPARTA;
	m_spartaConfig = config;
}

/**
 * Here we have to check in each round whether we have won.
 */
void LevelHandler::process(){
	// first determine whether we have to break

	if(m_started){
		bool breakoff = false;
		bool death = false;

		if(m_timeThresh == 0){
			m_timeRemaining = -1;
		} else {

			if(!m_paused){
				m_timeRemaining = m_timeThresh + m_timePaused - m_clock->getTimeMilliseconds()/1000;
			}

			if(m_timeRemaining <= 0){
				breakoff = true;
			}
		}

		m_points = m_pointingSystem->getPoints();

		if(m_pointThresh !=0){
			if(m_points >= m_pointThresh){
				breakoff = true;
			}
		}

		if(m_ballThresh == 0){
			m_ballsRemaining = -1;
		} else {
			int sumBalls = m_badHits + m_goodHits + m_reflectHits + m_notHit;
			m_ballsRemaining = m_ballThresh - sumBalls;

			if(m_ballsRemaining <= 0){
				breakoff = true;
			}
		}

		if(m_lifeThresh == 0){
			m_livesRemaining = -1;
		} else {
			m_livesRemaining = m_lifeThresh - m_badHits;

			if(m_livesRemaining <= 0){
				breakoff = true;
				death = true;
			}
		}

		if(breakoff){
			printf("breakoff\n");
			Message msg;
			int bonus = 0;
			if(death){
				bonus = 0;
			} else {
				if(m_timeRemaining > 0){
					m_bonusTime += m_timeRemaining*m_timeWeight ;
					bonus += m_bonusTime;
				}

				if(m_ballsRemaining > 0){
					m_bonusBalls += m_ballsRemaining*m_ballWeight;
					bonus += m_bonusBalls;
				}

				if(m_livesRemaining > 0){
					m_bonusLifes += m_livesRemaining*m_liveWeight;
					bonus += m_bonusLifes;
				}

			}
			msg.messageType = Message::MSG_WON;
			msg.additionalInfo = bonus;
			printf("Bonus points %i\n",bonus);
			sendMessage(msg);
			m_started = false;
//			printf("alles gemacht");
		}
	}
//	if(){
//		// breakTime
//	} else if(m_pointThresh > 0){
//
//	}
}

void LevelHandler::trigger(Message msg){
	switch (msg.messageType) {
		case Message::MSG_COL_PLAYER_FLY:

			switch(msg.additionalInfo){
				case FlyingObject::FOM_BAD :
					m_badHits++; m_notHit--; break;
				case FlyingObject::FOM_GOOD :
					m_goodHits++; m_notHit--; break;
				case FlyingObject::FOM_REFLECT :
					m_reflectHits++; m_notHit--; break;
				default : break;
			}
		break;

		case Message::MSG_SHOT_OBJECT:
			m_notHit++;
			break;

		case Message::MSG_START_GAME:
			m_paused = false;
			m_clock->reset();


			break;
		case Message::MSG_PAUSE_GAME:
			m_paused = true;
			m_pauseClock->reset();
			break;
		case Message::MSG_RETURN_GAME:
			m_timePaused = m_pauseClock->getTimeMilliseconds()/1000;
			m_paused = false;
			break;
	default:
		break;
	}
}

void LevelHandler::resetStatistics(){
	m_badHits = 0;
	m_goodHits = 0;
	m_reflectHits = 0;
}

int LevelHandler::getBadHits() const
{
    return m_badHits;
}

int LevelHandler::getGoodHits() const
{
    return m_goodHits;
}

int LevelHandler::getReflectHits() const
{
    return m_reflectHits;
}

int LevelHandler::getBallsRemaining() const
{
    return m_ballsRemaining;
}

int LevelHandler::getLivesRemaining() const
{
    return m_livesRemaining;
}

int LevelHandler::getPoints() const
{
    return m_points;
}

int LevelHandler::getTimeRemaining() const
{
    return m_timeRemaining;
}

void LevelHandler::setLivesRemaining(int m_livesRemaining)
{
    this->m_livesRemaining = m_livesRemaining;
}

std::list<std::string> LevelHandler::getDescriptionString() const
{
    return m_descriptionString;
}





//void LevelHandler::setBadHits(int badHits)
//{
//    this->badHits = badHits;
//}
//
//void LevelHandler::setGoodHits(int goodHits)
//{
//    this->goodHits = goodHits;
//}
//
//void LevelHandler::setReflectHits(int reflectHits)
//{
//    this->reflectHits = reflectHits;
//}






