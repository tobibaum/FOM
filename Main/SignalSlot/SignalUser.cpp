/*
 * PhysicalObject.cpp
 *
 *  Created on: May 28, 2011
 *      Author: Tobi
 */

#include "SignalUser.h"
#include <stdio.h>


SignalUser::SignalUser() {
	m_theStation = SignalStation::getInstance();
}

SignalUser::~SignalUser() {
	// TODO Auto-generated destructor stub
}

void SignalUser::sendMessage(Message msg){
	m_theStation->sendMessage(msg);
}

void SignalUser::setTrigger(Message::MessageType type){
	m_theStation->registerSlot(type, this);
}
