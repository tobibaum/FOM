/*
 * Object.cpp
 *
 *  Created on: May 11, 2011
 *      Author: Tobi
 */

#include "Object.h"
#include "ColoredWorld.h"

using namespace std;

Object::Object(){
	// This is just to allow FlyingObjects to have standard constructor.
}

Object::Object(btVector3 color, btCollisionObject* collObj, btScalar transparency, int type, btRigidBody* body){
	m_color = color;
	m_collObj = collObj;
	m_transparency = transparency;
	m_type = type;
	m_rigidBody = body;
	m_textureFile = "";

	if(m_type == COL_PLAYER){
		m_collided = true;
	}
}

Object::~Object() {

}

btCollisionObject *Object::getCollObj() const
{
    return m_collObj;
}

void Object::addTexture(string filename){
	m_textureFile = filename;
}

string Object::getTexture(){
	return m_textureFile;
}

btVector3 Object::getColor() const
{
    return m_color;
}

btScalar Object::getTransparency() const
{
    return m_transparency;
}

btVector3 Object::getDeltaLinearVelocity(){
	return m_rigidBody->getDeltaLinearVelocity();
}

btRigidBody* Object::getRigidBody(){
	return m_rigidBody;
}

void Object::setCollObj(btCollisionObject *m_collObj)
{
    this->m_collObj = m_collObj;
}

void Object::setColor(btVector3 m_color)
{
    this->m_color = m_color;
}

void Object::setTransparency(btScalar m_transparency)
{
    this->m_transparency = m_transparency;
}

int Object::getType() const
{
    return m_type;
}

bool Object::getCollided() const
{
    return m_collided;
}

void Object::setCollided(bool m_collided)
{
    this->m_collided = m_collided;
}

BodyParts::parts Object::getBodyPart() const
{
    return m_bodyPart;
}

void Object::setBodyPart(BodyParts::parts m_bodyPart)
{
    this->m_bodyPart = m_bodyPart;
}








