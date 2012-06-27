/*
 * CastlePointCounter.h
 *
 *  Created on: Jul 6, 2011
 *      Author: Tobi
 */

#ifndef CASTLEPOINTCOUNTER_H_
#define CASTLEPOINTCOUNTER_H_

#include "Castle.h"
#include "SignalUser.h"
#include <list>
#include <stdio.h>

class CastlePointCounter : public SignalUser{
private:
	CastlePointCounter();
	virtual ~CastlePointCounter();

public:

	void addCastle(Castle* castles);

	void deleteCastle(Castle* castles);

	float getPoints();

	void process();

	static CastlePointCounter* getInstance();

	static void destroy();

private:

	static CastlePointCounter* theInstance;

	std::list<Castle*> m_castles;

	float m_lastValue;

	int m_lastInt;

};

#endif /* CASTLEPOINTCOUNTER_H_ */
