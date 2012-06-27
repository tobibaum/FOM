/*
 * CakeObject.cpp
 *
 *  Created on: Jul 4, 2011
 *      Author: Tobi
 */

#include "CakeObject.h"
#include "CakeShape.h"
#include "ColoredWorld.h"
#include "Cleaner.h"

using namespace std;


CakeObject::CakeObject(btVector3 color, btCollisionObject* collObj, btScalar transparency, int type, btRigidBody* body)
: WallObject(color, collObj, transparency, type, body)
{
}


CakeObject::~CakeObject() {
	// TODO Auto-generated destructor stub
//	printf("delete cake\n");
}



void CakeObject::setCollidedW(bool collided)
{
//	printf("collidedW\n");
	float linVel = m_rigidBody->getLinearVelocity().z();
	m_collided = collided;

//	setColor(btVector3(1,0,0));
	list<triangle> theOneTri;
	theOneTri.push_back(((CakeShape*)m_rigidBody->getCollisionShape())->getTri());

	ColoredWorld* colWorld = ColoredWorld::getInstance();
	m_color = btVector3(1,1,1);
	LooseCompoundShape* shape = colWorld->addTriangles(m_rigidBody->getWorldTransform(),
			theOneTri,-3, linVel, this);

	list<pair<btRigidBody*, triangle> > rigids = shape->getRigidTris();

	if(rigids.size() != 0){
		m_parent->addRigids(rigids);
	}
	if(m_objMap.size() != 0){
		m_parent->addObjMap(m_objMap);
	}

//	ColoredWorld::getInstance()->deleteRigidBody(m_rigidBody);
}

void CakeObject::setCollidedW2(bool collided){

	m_collided = collided;
	m_rigidBody->setLinearVelocity(btVector3(0,0,-70));

	m_rigidBody->setGravity(btVector3(0,-100,0));
	float mass = 100;
	btVector3 localInertia;
	m_rigidBody->getCollisionShape()->calculateLocalInertia(mass, localInertia);
	m_rigidBody->setMassProps(mass,localInertia);

//	Cleaner::getInstance()->deleteBody(m_rigidBody,2000);
}

WallObject *CakeObject::getParent() const
{
    return m_parent;
}

void CakeObject::setParent(WallObject *m_parent)
{
    this->m_parent = m_parent;
}

CakeObject::CakeType CakeObject::getType() const
{
    return m_type;
}

void CakeObject::setType(CakeObject::CakeType m_type)
{
    this->m_type = m_type;
}




