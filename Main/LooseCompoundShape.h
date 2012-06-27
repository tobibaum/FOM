/*
 * LooseCompoundShape.h
 *
 *  Created on: Jun 29, 2011
 *      Author: Tobi
 */

#ifndef LOOSECOMPOUNDSHAPE_H_
#define LOOSECOMPOUNDSHAPE_H_
#include "btBulletDynamicsCommon.h"
#include <iostream>
#include <list>
#include <utility>
#include "Triangle.h"

class LooseCompoundShape : public btCompoundShape{
public:
	LooseCompoundShape();
	virtual ~LooseCompoundShape();
	void addChildShape(const btTransform& localTransform,btCollisionShape* shape);
	std::list<std::pair<btTransform*, btCollisionShape*> > getChildren();
	const char*	getName()const;
	void setRigids(std::list<btRigidBody*> bodies);
	void setVelocity(btVector3 vel);
	void setGravity(btVector3 grav);
    btRigidBody *getCenterRigid() const;
    void setCenterRigid(btRigidBody *m_centerRigid);
    std::list<btRigidBody*> getRigids() const;
    std::list<std::pair<btRigidBody*,triangle> > getRigidTris() const;
    void setRigidTris(std::list<std::pair<btRigidBody*,triangle> > m_rigidTris);

private:

	std::list<std::pair<btTransform*, btCollisionShape*> > m_children;
	std::list<std::pair<btRigidBody*, triangle> > m_rigidTris;
	std::list<btRigidBody*> m_bodies;
	btRigidBody* m_centerRigid;
};

#endif /* LOOSECOMPOUNDSHAPE_H_ */
