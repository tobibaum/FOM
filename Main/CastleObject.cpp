/*
 * CastleObject.cpp
 *
 *  Created on: Jul 6, 2011
 *      Author: Tobi
 */

#include "CastleObject.h"
#include "Cleaner.h"
using namespace std;


CastleObject::CastleObject(btVector3 color, btCollisionObject* collObj, btScalar transparency, int type, btRigidBody* body)
: Object (color, collObj, transparency, type, body)
{
	// TODO Auto-generated constructor stub
	m_pointFactor = 1;
	m_startHeight = body->getWorldTransform().getOrigin().getY();
	m_eraseThis = false;
}

CastleObject::~CastleObject() {
//	ColoredWorld::getInstance()->deleteRigidBody(m_body);
}

void CastleObject::deleteRigid(){
	ColoredWorld::getInstance()->deleteRigidBody(m_rigidBody);
}

bool CastleObject::getErase(){
	return m_eraseThis;
}

float CastleObject::getPoints(){
	float result = 0;


	btScalar currentPosition = m_rigidBody->getWorldTransform().getOrigin().getY();
	btScalar originalPosition = m_startHeight;

	result = m_pointFactor*(originalPosition - currentPosition);

	if(result < 0){
		result = 0;
	}

	if(currentPosition < 1){
//		printf("remove this \n");
		m_fatherCastle->removePart(this, result);
		m_eraseThis = true;
		Cleaner::getInstance()->deleteBody(m_rigidBody, 1000);
	}

	return result;
}

btScalar CastleObject::getPointFactor() const
{
    return m_pointFactor;
}

void CastleObject::setPointFactor(btScalar m_pointFactor)
{
    this->m_pointFactor = m_pointFactor;
}

void CastleObject::setFatherCastle(Castle *m_fatherCastle)
{
    this->m_fatherCastle = m_fatherCastle;
}




