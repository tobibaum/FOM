/*
 * FlyingObject.h
 *
 *  Created on: Jun 8, 2011
 *      Author: Tobi
 */

#ifndef FLYINGOBJECT_H_
#define FLYINGOBJECT_H_
#include "Object.h"
#include <list>
#include "BodyParts.h"
#include "SignalUser.h"
//#include "Player.h"



class FlyingObject : public Object, public SignalUser{
public:
	enum FlyingObjectType{
		FOM_BAD,
		FOM_GOOD,
		FOM_REFLECT,
		NUM_TYPE
	};

	FlyingObject(btVector3 color, btCollisionObject* collObj, btScalar transparency, int type, btRigidBody* body);
	virtual ~FlyingObject();

public:
	void collided(BodyParts::parts part);
	void collidedB(bool colli);
	void collided(bool colli);
    FlyingObjectType getType() const;
    void setType(FlyingObjectType type);
    bool getDestroyOnStruct() const;

private:

    bool m_destroyOnStruct;

    bool m_collidedB;

	FlyingObjectType type;

};

#endif /* FLYINGOBJECT_H_ */
