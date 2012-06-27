/*
 * SpartaCanon.cpp
 *
 *  Created on: Jul 4, 2011
 *      Author: Tobi
 */

#include "SpartaCannon.h"
#include <stdio.h>

using namespace std;


SpartaCannon::SpartaCannon() {
	m_running = false;
	m_coloredWorld = ColoredWorld::getInstance();
	m_shoot = true;
	setTrigger(Message::MSG_WALL_ERASED);

}

SpartaCannon::~SpartaCannon() {
	// TODO Auto-generated destructor stub
}

void SpartaCannon::trigger(Message msg){
	switch(msg.messageType){
	case Message::MSG_WALL_ERASED:
		m_shoot = true;
		break;
	}
}

void SpartaCannon::setConfiguration(SpartaConfiguration* config){
	m_config = config;
}

void SpartaCannon::updateLevelData(int level){
	SpartaLevel* newLevel = m_config->getLevel(level);
	m_frequency = newLevel->getFrequency();
	m_speed = newLevel->getSpeed();
	m_probabilities = newLevel->getLevelList();
	m_startPosition = *newLevel->getStartPosition();
}

void SpartaCannon::start(){
	m_running = true;
}

void SpartaCannon::stop(){
	m_running = false;
}

void SpartaCannon::process(){
	float ms = 1/m_frequency*1000;

	if(m_running){
		if(m_shoot){
			delete m_currentWall;

			btTransform startTrans;
			startTrans.setIdentity();
			startTrans.setOrigin(m_startPosition);

			string filename;

			int random = rand()%100 + 1;

			float probStack=0;
			for(list<WallProbability*>::iterator prob_it = m_probabilities.begin();
					prob_it != m_probabilities.end(); prob_it++){
				probStack += (*prob_it)->getProbability();
				if (random <= probStack){
					filename = (*prob_it)->getFilename();
					break;
				}
			}
			float depth  =  5; // we fix these
			float height = 30; // two to appropriate values

			m_currentWall = m_coloredWorld->addWallObject(startTrans,filename,depth,40);
			m_currentWall->setVelocity(btVector3(0,0,m_config->getLevel(1)->getSpeed()));

//			m_time_since_last_shot = 0;
			m_shoot = false;

			Message msg;
			msg.messageType = Message::MSG_SHOT_OBJECT;
			sendMessage(msg);
		}
//		else {
//			m_time_since_last_shot += get_time_delta_in_ms();
//		}
	}
}

btScalar SpartaCannon::get_time_delta_in_ms(){
	btScalar this_time = m_clock.getTimeMilliseconds();
	btScalar time_difference = this_time - m_last_time;
	m_last_time = this_time;
	return time_difference;
}
