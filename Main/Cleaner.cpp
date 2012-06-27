/*
 * Cleaner.cpp
 *
 *  Created on: Jun 9, 2011
 *      Author: Tobi
 */

#include "Cleaner.h"

using namespace std;

Cleaner* Cleaner::theInstance = 0;


Cleaner::Cleaner() {
	// TODO Auto-generated constructor stub
	m_coloredWorld = ColoredWorld::getInstance();

}

Cleaner::~Cleaner() {
	// TODO Auto-generated destructor stub
}

Cleaner* Cleaner::getInstance(){
	if(theInstance == 0){
		theInstance = new Cleaner();
	}

	return theInstance;
}

void Cleaner::deleteBody(btRigidBody* rigidBody, btScalar ms){
	btClock* now = new btClock();

	timedDeletion insert;
	insert.clock = now;
	insert.body = rigidBody;
	insert.ms_thresh = ms;

	m_stashedForDeletion.push_back(insert);
}

void Cleaner::process(){

	for(list<timedDeletion>::iterator del_it = m_stashedForDeletion.begin();
			del_it != m_stashedForDeletion.end(); del_it++){
		if((del_it->clock)->getTimeMilliseconds() > del_it->ms_thresh){

			m_coloredWorld->deleteRigidBody(del_it->body);

			delete del_it->clock;
			del_it = m_stashedForDeletion.erase(del_it);
		}
	}
}
