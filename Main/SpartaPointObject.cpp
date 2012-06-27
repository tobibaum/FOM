/*
 * SpartaPointClass.cpp
 *
 *  Created on: Jul 4, 2011
 *      Author: Tobi
 */

#include "SpartaPointObject.h"
#include "Cleaner.h"

SpartaPointObject::SpartaPointObject(btVector3 color, btCollisionObject* collObj, btScalar transparency, int type, btRigidBody* body)
: Object(color,collObj,transparency, type, body)
{
	// TODO Auto-generated constructor stub

}

SpartaPointObject::~SpartaPointObject() {
	// TODO Auto-generated destructor stub
}

void SpartaPointObject::setCollided(bool m_collided){
	this->m_collided = m_collided;
	m_rigidBody->setGravity(btVector3(0,-100,-20));
	Cleaner::getInstance()->deleteBody(m_rigidBody,500);


}

void SpartaPointObject::setCollidedImmediate(bool m_collided){
	this->m_collided = m_collided;

	Cleaner::getInstance()->deleteBody(m_rigidBody,0);
}
