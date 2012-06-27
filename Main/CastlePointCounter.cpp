/*
 * CastlePointCounter.cpp
 *
 *  Created on: Jul 6, 2011
 *      Author: Tobi
 */

#include "CastlePointCounter.h"

using namespace std;

CastlePointCounter* CastlePointCounter::theInstance = 0;

CastlePointCounter::CastlePointCounter() {

	m_lastInt = 0;
	m_lastValue = 0;

}

CastlePointCounter::~CastlePointCounter() {

	for(list<Castle*>::iterator castle_it = m_castles.begin();
			castle_it != m_castles.end(); castle_it++){
		delete *castle_it;
	}

	m_castles.clear();
}

CastlePointCounter* CastlePointCounter::getInstance(){
	if(theInstance == 0){
		theInstance = new CastlePointCounter;
	}

	return theInstance;
}

void CastlePointCounter::destroy(){
	if(theInstance!=0){
		delete theInstance;
	}

	theInstance = 0;
}

void CastlePointCounter::addCastle(Castle* castles){
	m_castles.push_back(castles);
}

void CastlePointCounter::deleteCastle(Castle* castles){
	m_castles.remove(castles);
}

void CastlePointCounter::process(){
	float result = 0;

	for(list<Castle*>::iterator castle_it = m_castles.begin();
			castle_it != m_castles.end(); castle_it++){
		result += (*castle_it)->getPoints();
	}

	int resultInt = (int)result;
	int swapInt = resultInt;
	resultInt -= m_lastInt;
	m_lastInt = swapInt;

	if(resultInt > 0){
		// the value changed, we want to indicate that with a Message
		Message msg;
		msg.messageType = Message::MSG_CASTLE_POINTS;
		msg.additionalInfo = resultInt;
		sendMessage(msg);
	}

}

float CastlePointCounter::getPoints(){
	return m_lastValue;
}
