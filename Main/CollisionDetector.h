/*
 * CollisionDetector.h
 *
 *  Created on: May 24, 2011
 *      Author: Tobi
 */

#ifndef COLLISIONDETECTOR_H_
#define COLLISIONDETECTOR_H_

#include "btBulletDynamicsCommon.h"
#include "ColoredWorld.h"
#include "SignalStation.h"
#include "SignalUser.h"
#include "LetterCompoundShape.h"
//#include "LooseCompoundShape.h"

class ColoredWorld;

// This class handles collisions and responds with all colls that have to be handled.
class CollisionDetector : public SignalUser{
	friend class ColoredWorld;
private:
	CollisionDetector();
	virtual ~CollisionDetector();

public:
	static CollisionDetector* getInstance();
	static void destroy();
	void process();

private: /* functions */
	void collisionDetection();

private: /* members */
	static CollisionDetector* m_theInstance;
	btDynamicsWorld* m_dynamicsWorld;

	SignalStation* m_signalStation;
	btTransform* last_word_trans;
	LetterCompoundShape* last_looseCompound;
	ColoredWorld* m_coloredWorld;
	int m_wall_coll;
};

#endif /* COLLISIONDETECTOR_H_ */

