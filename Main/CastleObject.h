/*
 * CastleObject.h
 *
 *  Created on: Jul 6, 2011
 *      Author: Tobi
 */

#ifndef CASTLEOBJECT_H_
#define CASTLEOBJECT_H_
#include "Object.h"
#include "btBulletDynamicsCommon.h"
#include "Castle.h"

class Castle;

class CastleObject : public Object{
public:
	CastleObject(btVector3 color, btCollisionObject* collObj, btScalar transparency, int type, btRigidBody* body);
	virtual ~CastleObject();

	float getPoints();
    btScalar getPointFactor() const;
    void setPointFactor(btScalar m_pointFactor);

    void setFatherCastle(Castle *m_fatherCastle);

    void deleteRigid();

    bool getErase();

private:

	btScalar m_pointFactor;

	btRigidBody* m_body;

	float m_startHeight;

	Castle* m_fatherCastle;

	bool m_eraseThis;

};

#endif /* CASTLEOBJECT_H_ */
