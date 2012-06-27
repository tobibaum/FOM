/*
 * SpartaPointClass.h
 *
 *  Created on: Jul 4, 2011
 *      Author: Tobi
 */

#ifndef SPARTAPOINTOBJECT_H_
#define SPARTAPOINTOBJECT_H_

#include "Object.h"
#include "SignalUser.h"

class SpartaPointObject : public Object, public SignalUser{
public:
	SpartaPointObject(btVector3 color, btCollisionObject* collObj, btScalar transparency, int type, btRigidBody* body);
	virtual ~SpartaPointObject();

	void setCollided(bool m_collided);
	void setCollidedImmediate(bool m_collided);

};

#endif /* SPARTAPOINTCLASS_H_ */
