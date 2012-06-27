/*
 * Player.h
 *
 *  Created on: Apr 28, 2011
 *      Author: hendrik
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "btBulletDynamicsCommon.h"
#include "math.h"
#include "Skeleton.h"
#include "CollisionClasses.h"
#include "ColoredWorld.h"
#include "SignalUser.h"
#include "BodyParts.h"
#include "Object.h"

class Player : public SignalUser{
public:
	Player(ColoredWorld* ownerWorld);
	virtual ~Player();

//	typedef bodyParts BodyParts::parts;


	void trigger(Message msg);

	BodyParts::parts getBodyPart(btCollisionShape* body);

	void createBodyShapes();
	void updatePlayer();
	void updateInit();
	static void point_update_init();
private:

	void checkPausePressed();

	void allocateBodyPartEnum();

	user computeJump(user u);

	btTransform getTransformZ(btVector3* origin, btVector3* target, float* length);

	void createHead(btVector3* head);
	void createChest(btVector3* chest, btVector3* rShoulder, btVector3* lShoulder,
			 	 	 btVector3* rPelvis, btVector3* lPelvis);
	void createUpperArms(btVector3* lShoulder, btVector3* rShoulder, btVector3* lElbow, btVector3* rElbow);
	void createLowerArms(btVector3* rElbow, btVector3* lElbow, btVector3* rHand, btVector3* lHand);
	void createUpperLegs(btVector3* rPelvis, btVector3* lPelvis, btVector3* rKnee, btVector3* lKnee);
	void createLowerLegs(btVector3* rKnee, btVector3* lKnee, btVector3* rFoot, btVector3* lFoot);

	void createCapsule(int bodyIndex, btTransform* transform, float radius, float height, btVector3 color, btScalar transparency=0);
	void createCylinder(int bodyIndex, btTransform* transform, float radius, float height, btVector3 color, btScalar transparency=0);
	void createSphere(int bodyIndex, btTransform* transform, float radius, btVector3 color, btScalar transparency=0);

	btQuaternion* calculateRotation(btVector3* axis, btVector3* targetAxis);

	float getAdaptedRadius();

	ColoredWorld* m_ownerWorld;
	btCollisionShape* m_shapes[BodyParts::BODY_COUNT];
	btRigidBody* m_bodies[BodyParts::BODY_COUNT];

	short int m_armsCollideWith;
	Skeleton* m_skeleton;


	btScalar m_transparency;
	btVector3 m_color;

	user m_user;

	bool m_justPushedPause;

	btClock* m_pauseClock;

	btScalar m_pauseTimeThreshMs;

};

#endif /* PLAYER_H_ */
