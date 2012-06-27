/*
 * PointingSystem.cpp
 *
 *  Created on: May 29, 2011
 *      Author: Tobi
 */

#include "PointingSystem.h"
#include <stdio.h>
#include "FlyingObject.h"
#include "CastlePointCounter.h"

PointingSystem* PointingSystem::theInstance = 0;

PointingSystem::PointingSystem() {
	setTrigger(Message::MSG_COL_PLAYER_FLY);
	setTrigger(Message::MSG_START_GAME);
	setTrigger(Message::MSG_END_GAME);
	setTrigger(Message::MSG_CASTLE_POINTS);
	setTrigger(Message::MSG_COL_FLYING_BOUNDARY);
	setTrigger(Message::MSG_SPARTA_POINTS);
	setTrigger(Message::MSG_NEXT_LEVEL);
	setTrigger(Message::MSG_WON);
	m_points = 0;
	m_running = false;

}

PointingSystem* PointingSystem::getInstance(){
	if(theInstance == 0){
		theInstance = new PointingSystem;
	}

	return theInstance;
}

void PointingSystem::destroy(){
	if(theInstance != 0){
		delete theInstance;
	}

	theInstance = 0;
}

PointingSystem::~PointingSystem() {
	// TODO Auto-generated destructor stub
}

void PointingSystem::trigger(Message msg){

	switch(msg.messageType){

	case Message::MSG_START_GAME:
		m_points = 0;
		m_overall_points = 0;
//		printf("game started!\n");
		m_running = true;
		break;

	case Message::MSG_WON:
		m_overall_points += msg.additionalInfo;
		break;

	case Message::MSG_COL_PLAYER_FLY:

		if(m_running){
			switch(msg.additionalInfo){
			case FlyingObject::FOM_BAD:
				m_points -= 10;
				break;
			case FlyingObject::FOM_GOOD:
				m_points += 10;
				break;

			case FlyingObject::FOM_REFLECT:
				m_points += 0;
				break;
			}
		}
		break;

	case Message::MSG_END_GAME:
		m_overall_points += m_points;
		m_points = 0;
		break;

	case Message::MSG_NEXT_LEVEL:
		m_overall_points += m_points;
		m_points = 0;
		break;

	case Message::MSG_CASTLE_POINTS:
		m_points += msg.additionalInfo;
		break;

	case Message::MSG_COL_FLYING_BOUNDARY:
		m_points += msg.additionalInfo;
		break;

	case Message::MSG_SPARTA_POINTS:
		m_points += msg.additionalInfo;
		break;
	}

//	if (m_points >= 50000){
//		// We won!!
//		Message msg;
//		msg.messageType = Message::MSG_WON;
//		msg.additionalInfo = m_points;
//		sendMessage(msg);
////		m_points = 0;
//	}
}

int PointingSystem::getPoints() const
{
    return m_points;
}

void PointingSystem::setPoints(int m_points)
{
    this->m_points = m_points;
}

int PointingSystem::getOverallPoints() const
{
    return m_overall_points;
}




