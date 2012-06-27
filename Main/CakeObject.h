/*
 * CakeObject.h
 *
 *  Created on: Jul 4, 2011
 *      Author: Tobi
 */

#ifndef CAKEOBJECT_H_
#define CAKEOBJECT_H_

#include "WallObject.h"
#include "SignalUser.h"
#include <list>

class CakeObject : public WallObject, public SignalUser {
public:
	enum CakeType{
		CAKE_TYPE_WALL,
		CAKE_TYPE_HOLE
	};

	CakeObject(btVector3 color, btCollisionObject* collObj, btScalar transparency, int type, btRigidBody* body);
	virtual ~CakeObject();

	void setCollidedW(bool m_collided);
	void setCollidedW2(bool collided);
	WallObject *getParent() const;
    void setParent(WallObject *m_parent);
    CakeType getType() const;
    void setType(CakeType m_type);

private:

	WallObject* m_parent;

	CakeType m_type;
};

#endif /* CAKEOBJECT_H_ */
