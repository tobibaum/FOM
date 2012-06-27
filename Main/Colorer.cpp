/*
 * Colorer.cpp
 *
 *  Created on: Jun 9, 2011
 *      Author: Tobi
 */

#include "Colorer.h"

using namespace std;

Colorer* Colorer::theInstance = 0;


Colorer::Colorer() {
	// TODO Auto-generated constructor stub
	m_coloredWorld = ColoredWorld::getInstance();

}

Colorer::~Colorer() {
	// TODO Auto-generated destructor stub
}

Colorer* Colorer::getInstance(){
	if(theInstance == 0){
		theInstance = new Colorer();
	}

	return theInstance;
}

void Colorer::colorBody(Object* object, btScalar ms, btVector3 color){
	btClock* now = new btClock();

	timedColoring insert;
	insert.clock = now;
	insert.object = object;
	insert.ms_thresh = ms;
	insert.origColor = object->getColor();

	object->setColor(color);

	m_stashedForColoring.push_back(insert);
}

void Colorer::process(){

	for(list<timedColoring>::iterator color_it = m_stashedForColoring.begin();
			color_it != m_stashedForColoring.end(); color_it++){
		if((color_it->clock)->getTimeMilliseconds() > color_it->ms_thresh){
			color_it->object->setColor(color_it->origColor);
			color_it = m_stashedForColoring.erase(color_it);
		}
	}
}
