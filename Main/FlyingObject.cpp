/*
 * FlyingObject.cpp
 *
 *  Created on: Jun 8, 2011
 *      Author: Tobi
 */

#include "FlyingObject.h"
#include "ColoredWorld.h"
#include "Cleaner.h"
#include "HandTracker.h"

#define KICKPENALTY -2

FlyingObject::FlyingObject(btVector3 color, btCollisionObject* collObj, btScalar transparency, int type, btRigidBody* body)
: Object(color, collObj, transparency, type, body)
{
	// TODO Auto-generated constructor stub
	m_destroyOnStruct = false;
	m_collidedB = false;
}

FlyingObject::~FlyingObject() {
	// TODO Auto-generated destructor stub
}

void FlyingObject::collided(bool colli){
	m_collided = colli;

	switch (type) {
		case FlyingObject::FOM_REFLECT:
//			m_rigidBody->setLinearVelocity(btVector3(-formerLinear.x(),-formerLinear.y(),-formerLinear.z()*5));
			Cleaner::getInstance()->deleteBody(m_rigidBody,500);
			break;
		case FlyingObject::FOM_GOOD:
			Cleaner::getInstance()->deleteBody(m_rigidBody,0);
			break;
		default:
			// delete this object!
			Cleaner::getInstance()->deleteBody(m_rigidBody,0);
			break;
	}
}

void FlyingObject::collidedB(bool colli){
	m_collided = colli;


	if(m_destroyOnStruct && !m_collidedB){
		// This object already collided with the player, now it collided with a boundary, that must give a point
		Message msg;
		msg.messageType = Message::MSG_COL_FLYING_BOUNDARY;
		btVector3 position = m_rigidBody->getWorldTransform().getOrigin();
		position.setY(0);
		msg.additionalInfo = (int)position.length()*0.1;
//		printf("boundPoints: %d\n",(int)position.length()*0.2);
		m_collidedB = true;
		sendMessage(msg);
	}

	switch (type) {
		case FlyingObject::FOM_REFLECT:
//			m_rigidBody->setLinearVelocity(btVector3(-formerLinear.x(),-formerLinear.y(),-formerLinear.z()*5));
			Cleaner::getInstance()->deleteBody(m_rigidBody,500);
			break;
		case FlyingObject::FOM_GOOD:
			Cleaner::getInstance()->deleteBody(m_rigidBody,0);
			break;
		default:
			// delete this object!
			Cleaner::getInstance()->deleteBody(m_rigidBody,0);
			break;
	}
}


void FlyingObject::collided(BodyParts::parts part){
	m_collided = true;
	Skeleton* usr = Skeleton::getInstance();
	btVector3 formerLinear = m_rigidBody->getLinearVelocity();

	btVector3 partPos;
	btVector3 moveDir;
	btScalar speed;

	bool getsBoosted = false;
	HandTracker* tracker = HandTracker::getInstance();
	switch(part){
	case BodyParts::UPPER_ARM_R : // the same case as below
	case BodyParts::LOWER_ARM_R : partPos = usr->getSkeleton().hand_right;
								  moveDir = *(tracker->getMoveDirection(HandTracker::TRACK_R_HAND));
								  speed = tracker->getSpeed(HandTracker::TRACK_R_HAND);
								  getsBoosted = true;
								  break;
	case BodyParts::UPPER_ARM_L : // the same case as below
	case BodyParts::LOWER_ARM_L : partPos = usr->getSkeleton().hand_left;
								  moveDir = *(tracker->getMoveDirection(HandTracker::TRACK_L_HAND));
								  speed = tracker->getSpeed(HandTracker::TRACK_L_HAND);
								  getsBoosted = true;
								  break;
	case BodyParts::UPPER_LEG_R : // the same case as below
	case BodyParts::LOWER_LEG_R : partPos = usr->getSkeleton().foot_right;
								  moveDir = *(tracker->getMoveDirection(HandTracker::TRACK_R_FOOT));
								  speed = tracker->getSpeed(HandTracker::TRACK_R_FOOT);
								  getsBoosted = true;
								  moveDir = moveDir + btVector3(0,KICKPENALTY,0);
								  break;
	case BodyParts::UPPER_LEG_L : // the same case as below
	case BodyParts::LOWER_LEG_L : partPos = usr->getSkeleton().foot_left;
								  moveDir = *(tracker->getMoveDirection(HandTracker::TRACK_L_FOOT));
								  speed = tracker->getSpeed(HandTracker::TRACK_L_FOOT);
								  getsBoosted = true;
								  moveDir = moveDir + btVector3(0,KICKPENALTY,0);
								  break;
	default : getsBoosted = false; break;
	}

	switch (type) {
		case FlyingObject::FOM_REFLECT:
//			m_rigidBody->setLinearVelocity(btVector3(-formerLinear.x(),-formerLinear.y(),-formerLinear.z()*5));
			if(getsBoosted){
				m_rigidBody->setLinearVelocity(((m_rigidBody->getWorldTransform().getOrigin() - partPos) + moveDir)*(speed));

			}
			m_destroyOnStruct = true;
//			Cleaner::getInstance()->deleteBody(m_rigidBody,500);
			break;
		case FlyingObject::FOM_GOOD:
			Cleaner::getInstance()->deleteBody(m_rigidBody,500);
			break;
		default:
			// delete this object!
			Cleaner::getInstance()->deleteBody(m_rigidBody,500);
			break;
	}
}

FlyingObject::FlyingObjectType FlyingObject::getType() const
{
    return type;
}

void FlyingObject::setType(FlyingObjectType type)
{
    this->type = type;
}

bool FlyingObject::getDestroyOnStruct() const
{
    return m_destroyOnStruct;
}


