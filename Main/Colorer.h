/*
 * Colorer.h
 *
 *  Created on: Jun 9, 2011
 *      Author: Tobi
 */

#ifndef COLORER_H_
#define COLORER_H_

#include <list>
#include <string>
#include <stdio.h>
#include "ColoredWorld.h"


class Colorer {
private:

	struct timedColoring{
		btClock* clock;
		btScalar ms_thresh;
		Object* object;
		btVector3 origColor;
	};

	Colorer();
	virtual ~Colorer();

public:
	void colorBody(Object* object, btScalar ms, btVector3 color);

	void process();

	static Colorer* getInstance();

public:
	static Colorer* theInstance;

private:

	std::list<timedColoring> m_stashedForColoring;
	ColoredWorld* m_coloredWorld;
};

#endif /* Colorer_H_ */
