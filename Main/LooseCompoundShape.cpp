/*
 * LooseCompoundShape.cpp
 *
 *  Created on: Jun 29, 2011
 *      Author: Tobi
 */

#include "LooseCompoundShape.h"
#include "ColoredWorld.h"
using namespace std;

LooseCompoundShape::LooseCompoundShape() {
	// TODO Auto-generated constructor stub

}

LooseCompoundShape::~LooseCompoundShape() {
	// TODO Auto-generated destructor stub

	for(list<pair<btTransform*, btCollisionShape*> >::iterator pair_it = m_children.begin();
			pair_it != m_children.end(); pair_it++){
		delete pair_it->first;
		delete pair_it->second;
	}

//	m_rigidTris;
	for(list<btRigidBody*>::iterator rigid_it = m_bodies.begin();
			rigid_it != m_bodies.end(); rigid_it++){
		ColoredWorld::getInstance()->deleteRigidBody(*rigid_it);
	}

	m_children.clear();
	m_rigidTris.clear();
	m_bodies.clear();

	ColoredWorld::getInstance()->deleteRigidBody(m_centerRigid);
}

void LooseCompoundShape::addChildShape(const btTransform& localTransform,btCollisionShape* shape){
	pair<btTransform*,btCollisionShape*> newPair;
	btTransform* trans = new btTransform(localTransform);
	newPair.first = trans;
	newPair.second = shape;
	m_children.push_back(newPair);
}

const char* LooseCompoundShape::getName() const{
	return "LooseCompound";
}

void LooseCompoundShape::setRigids(list<btRigidBody*> bodies){
	m_bodies = bodies;
}

void LooseCompoundShape::setVelocity(btVector3 vel){
	for(list<btRigidBody*>::iterator body_it = m_bodies.begin();
			body_it != m_bodies.end(); body_it++){
		(*body_it)->setLinearVelocity(vel);
	}
	m_centerRigid->setLinearVelocity(vel);
}

void LooseCompoundShape::setGravity(btVector3 grav){
	for(list<btRigidBody*>::iterator body_it = m_bodies.begin();
			body_it != m_bodies.end(); body_it++){
		(*body_it)->setGravity(grav);
	}
	m_centerRigid->setGravity(grav);
}

list<pair<btTransform*, btCollisionShape*> > LooseCompoundShape::getChildren(){
	return m_children;
}

btRigidBody *LooseCompoundShape::getCenterRigid() const
{
    return m_centerRigid;
}

void LooseCompoundShape::setCenterRigid(btRigidBody *m_centerRigid)
{
	this->m_centerRigid = m_centerRigid;
}

std::list<btRigidBody*> LooseCompoundShape::getRigids() const
{
    return m_bodies;
}

std::list<std::pair<btRigidBody*,triangle> > LooseCompoundShape::getRigidTris() const
{
    return m_rigidTris;
}

void LooseCompoundShape::setRigidTris(std::list<std::pair<btRigidBody*,triangle> > m_rigidTris)
{
    this->m_rigidTris = m_rigidTris;
}






