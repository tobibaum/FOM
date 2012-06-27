/*
 * Hendrik.h
 *
 *  Created on: May 23, 2011
 *      Author: Tobi
 */

#ifndef HENDRIK_H_
#define HENDRIK_H_

#include "Field.h"
#include "MenuList.h"
#include "CollisionDetector.h"
#include "GameConfigurations/ConfigLoader.h"
#include "HighscoreHandler.h"
//#include "ShapeFactory.h"

class Hendrik: public Field {
public:
	Hendrik();
	virtual ~Hendrik();

	void initWorld();

private:
	void step();

	ShapeFactory m_shapeFac;

	int rotateTest;

	ConfigLoader* m_confLoader;
	HighscoreHandler* m_highscoreHandler;
};

#endif /* HENDRIK_H_ */
