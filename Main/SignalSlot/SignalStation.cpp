/*
 * SignalStation.cpp
 *
 *  Created on: May 28, 2011
 *      Author: Tobi
 */

#include "SignalStation.h"
#include <stdio.h>

using namespace std;

SignalStation* SignalStation::m_theInstance = 0;

SignalStation::SignalStation() {
	list<SignalUser*> empty_list;
	for(int i = 0; i <= Message::NUM_MSG;i++){
		m_slots[(Message::MessageType)i] = empty_list;
	}
}

SignalStation::~SignalStation() {
	// TODO Auto-generated destructor stub
}

SignalStation* SignalStation::getInstance(){
	if(!m_theInstance){
		m_theInstance = new SignalStation();
	}
	return m_theInstance;
}

void SignalStation::destroy(){
	if(m_theInstance){
		delete m_theInstance;
	}
	m_theInstance = 0;
}

void SignalStation::sendMessage(Message msg){
	m_queue.push_back(msg);
}

void SignalStation::registerSlot(Message::MessageType type, SignalUser* obj){
	m_slots[type].push_back(obj);
}

void SignalStation::process(){
	if(!m_slots.empty()){
		while(!m_queue.empty()){
	//		printf("queue is non-empty.\n");
			Message current = m_queue.front();

			list<SignalUser*> objects = m_slots.at(current.messageType);

			for(list<SignalUser*>::iterator obj_it = objects.begin();
					obj_it != objects.end(); obj_it++){
				(*obj_it)->trigger(current);
			}

			m_queue.pop_front();
		}
	}
}
