/*
 * Castle.h
 *
 *  Created on: May 4, 2011
 *      Author: Tobi
 *
 *  This class is meant to hold the geometric object of a castle to be destroyed.
 */

#ifndef CASTLE_H_
#define CASTLE_H_

#include "btBulletDynamicsCommon.h"
#include "CastleObject.h"
#include "ColoredWorld.h"
#include "CollisionClasses.h"

class CastleObject;

class Castle {
public:
	Castle(int in_x, int in_z);
	virtual ~Castle();

	float getPoints();
    btScalar getPointFactor() const;
    void setPointFactor(btScalar m_pointFactor);
    void removePart(CastleObject* part, float points);

private:

    void create(int x, int z);

    void doRemoval();

	ColoredWorld* m_coloredWorld;

	std::list<CastleObject*> m_parts;

	std::list<CastleObject*> m_stashedForErase;

	btScalar m_pointFactor;

	float m_basicValue;
};

#endif /* CASTLE_H_ */
