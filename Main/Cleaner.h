/*
 * Cleaner.h
 *
 *  Created on: Jun 9, 2011
 *      Author: Tobi
 */

#ifndef CLEANER_H_
#define CLEANER_H_

#include <list>
#include <string>
#include <stdio.h>
#include "ColoredWorld.h"


class Cleaner {
private:

	struct timedDeletion{
		btClock* clock;
		btScalar ms_thresh;
		btRigidBody* body;
	};

	Cleaner();
	virtual ~Cleaner();

public:
	void deleteBody(btRigidBody* rigidBody, btScalar ms);

	void process();

	static Cleaner* getInstance();

public:
	static Cleaner* theInstance;

private:

	std::list<timedDeletion> m_stashedForDeletion;
	ColoredWorld* m_coloredWorld;
};

#endif /* CLEANER_H_ */
