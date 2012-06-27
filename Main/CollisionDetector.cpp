/*
 * CollisionDetector.cpp
 *
 *  Created on: May 24, 2011
 *      Author: Tobi
 */

#include "CollisionDetector.h"
#include "CakeObject.h"
#include "SpartaPointObject.h"
#include "BodyParts.h"
#include "Colorer.h"

using namespace std;

CollisionDetector* CollisionDetector::m_theInstance = 0;


CollisionDetector::CollisionDetector() {
	m_dynamicsWorld = ColoredWorld::getInstance()->getDynamicsWorld();
	m_signalStation = SignalStation::getInstance();
	m_coloredWorld = ColoredWorld::getInstance();
	int m_wall_coll = 0;
}

CollisionDetector::~CollisionDetector() {
	// TODO Auto-generated destructor stub
}

CollisionDetector* CollisionDetector::getInstance(){
	if(!m_theInstance){
		m_theInstance = new CollisionDetector();
	}
	return m_theInstance;
}

void CollisionDetector::destroy(){
	if(m_theInstance){
		delete m_theInstance;
	}

	m_theInstance = 0;
}

void CollisionDetector::process(){
	collisionDetection();
}

/**
 * This is the main part of the collision Detection
 */
void CollisionDetector::collisionDetection(){
	// Recognize and handle collisions
	int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();

	btVector3 deltaLinVel = btVector3(0,0,0);
	int colls = 0;

	for(int i = 0; i < numManifolds;i++){
		btPersistentManifold* contact = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obj0 = static_cast<btCollisionObject*>(contact->getBody0());
		btCollisionObject* obj1 = static_cast<btCollisionObject*>(contact->getBody1());

		Object* object0 = m_coloredWorld->getObject(obj0);
		Object* object1 = m_coloredWorld->getObject(obj1);

		if( (object0->getType() | object1->getType() ) == (COL_PLAYER | COL_FLYING_OBJECTS)){

			if(	!object0->getCollided() || !object1->getCollided()) {
				deltaLinVel += object0->getDeltaLinearVelocity().absolute() + object1->getDeltaLinearVelocity().absolute();

				if((deltaLinVel.length() > 0) ){
					Message msg;

					object0->setCollided(true);
					object1->setCollided(true);

					BodyParts::parts bodyPart;

					FlyingObject* flyingObj;
					Object* bodyObj;
					if(object0->getType() == COL_FLYING_OBJECTS){
						flyingObj = (FlyingObject*)object0;
//						bodyPart = obj0->getCollisionShape();
						bodyPart = object1->getBodyPart();
						bodyObj = object1;
					} else {
						bodyPart = object0->getBodyPart();
						flyingObj = (FlyingObject*)object1;
						bodyObj = object0;
					}

//					Colorer::getInstance()->colorBody(bodyObj, 500,flyingObj->getColor());

					msg.additionalInfo = flyingObj->getType();
					flyingObj->collided(bodyPart);

					msg.messageType = Message::MSG_COL_PLAYER_FLY;
					m_signalStation->sendMessage(msg);

				}
			}

		}/* else if( (object0->getType() | object1->getType() ) == (COL_PLAYER | COL_LETTERS) &&
							(!object0->getCollided() ||  !object1->getCollided())) {

			LetterCompoundShape* looseCompound;

			if (object0->getType() == COL_LETTERS) {
				looseCompound = (LetterCompoundShape*)object0->getCollObj()->getCollisionShape();
			} else {
				looseCompound = (LetterCompoundShape*)object1->getCollObj()->getCollisionShape();
			}
			btTransform* loose_trans = new btTransform();
			btVector3* aabb_min = new btVector3();
			btVector3* aabb_max = new btVector3();

			// now we changed.
			last_looseCompound = looseCompound;
			last_word_trans = loose_trans;
			Message msg;
			msg.additionalInfo = 0;
			msg.menuPointWord = looseCompound->getType();
			msg.messageType = Message::MSG_COL_PLAYER_LETTER;
			m_signalStation->sendMessage(msg);

			object0->setCollided(true);
			object1->setCollided(true);
		}*/ else if( (object0->getType() | object1->getType() ) == (COL_PLAYER | COL_WALL) &&
				!(object0->getCollided() && object1->getCollided())){


//			if(object0->getType() == COL_WALL){
//
//			}
//
//			m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
//			int conts = contact->getNumContacts();
//			m_wall_coll += conts;
//
//			if(conts > 0){
//				btManifoldPoint cont_point = contact->getContactPoint(0);
//				btVector3 coll_pos = cont_point.getPositionWorldOnA();
//			}

		}else if( (object0->getType() | object1->getType() ) == (COL_PLAYER | COL_CAKE)){

			if(	!object0->getCollided() || !object1->getCollided()) {

				if(((object0->getDeltaLinearVelocity().absolute() + object1->getDeltaLinearVelocity().absolute()).length() > 0) ){
					Message msg;

					if(object0->getType() == COL_CAKE){
						((CakeObject*)object0)->setCollidedW(true);
						object1->setCollided(true);
					} else {
						((CakeObject*)object1)->setCollidedW(true);
						object0->setCollided(true);
					}
				}
			}
		} else if((object0->getType() | object1->getType() ) == (COL_PLAYER | COL_FINAL_CAKE)){
			if(	!object0->getCollided() || !object1->getCollided()) {

				if(((object0->getDeltaLinearVelocity().absolute() + object1->getDeltaLinearVelocity().absolute()).length() > 0) ){

					if(object0->getType() == COL_FINAL_CAKE){
						((CakeObject*)object0)->setCollidedW2(true);
						object1->setCollided(true);
					} else {
						((CakeObject*)object1)->setCollidedW2(true);
						object0->setCollided(true);
					}

					Message msg;
					msg.messageType = Message::MSG_SPARTA_POINTS;
					msg.additionalInfo = -30;
					sendMessage(msg);
				}
			}
		}else if( (object0->getType() | object1->getType() ) == (COL_PLAYER | COL_SPARTA_POINTS) &&
				!(object0->getCollided() & object1->getCollided())){
			if(((object0->getDeltaLinearVelocity().absolute() + object1->getDeltaLinearVelocity().absolute()).length() > 0) ){
				object0->setCollided(true);
				object1->setCollided(true);
				Message msg;
				msg.messageType = Message::MSG_SPARTA_POINTS;
				msg.additionalInfo = 10;
				sendMessage(msg);
			}
		}

		else if( (object0->getType() | object1->getType() ) == (COL_BOUNDARY | COL_SPARTA_POINTS) &&
				!(object0->getCollided() && object1->getCollided())){

			if(object0->getType() == COL_SPARTA_POINTS){
				((SpartaPointObject*)object0)->setCollidedImmediate(true);
				object1->setCollided(true);
			}else{
				((SpartaPointObject*)object1)->setCollidedImmediate(true);
				object0->setCollided(true);
			}
		}

		else if( (object0->getType() | object1->getType() ) == (COL_BOUNDARY | COL_FLYING_OBJECTS) &&
				!(object0->getCollided() && object1->getCollided())){
			if(object0->getType() == COL_FLYING_OBJECTS){
				((FlyingObject*)object0)->collidedB(true);
			}else {
				((FlyingObject*)object1)->collidedB(true);
			}

		}

		else if( (object0->getType() | object1->getType() ) == (COL_STRUCTURES | COL_FLYING_OBJECTS) &&
						!(object0->getCollided() && object1->getCollided())){

			FlyingObject* flyingObj;
			if(object0->getType() == COL_FLYING_OBJECTS){
				flyingObj = (FlyingObject*)object0;
			}else {
				flyingObj = (FlyingObject*)object1;
			}

			if(flyingObj->getDestroyOnStruct()){
				flyingObj->collided(true);
			}

		}

		contact->clearManifold();
	}


	/*
	 * Do the supplementary detection for letters!
	 */

	btRigidBody* body;
	list<Object*> letters = m_coloredWorld->getLetterObjects();
	for(list<Object*>::iterator obj_it = letters.begin(); obj_it != letters.end();
			obj_it++){
		body = (*obj_it)->getRigidBody();
		if((body->getLinearVelocity().absolute().length() != 0 ||
			body->getAngularVelocity().absolute().length() != 0	) &&
			!(*obj_it)->getCollided()){
			(*obj_it)->setCollided(true);
			Message msg;
			msg.additionalInfo = 0;
			msg.menuPointWord = ((LetterCompoundShape*)(*obj_it)->getCollObj()->getCollisionShape())->getType();
			msg.messageType = Message::MSG_COL_PLAYER_LETTER;
			m_signalStation->sendMessage(msg);
		}

	}
}

