/*
 * Player.cpp
 *
 *  Created on: Apr 28, 2011
 *      Author: hendrik
 */

#include "Player.h"

Player::Player(ColoredWorld* ownerWorld) {
	// TODO Auto-generated constructor stub
	m_ownerWorld = ownerWorld;
	m_skeleton = Skeleton::getInstance();
	m_transparency = 0.6;
	m_color = btVector3(0.f,0.3f,1.f);
	m_justPushedPause = false;
	m_pauseClock = 0;
	m_pauseTimeThreshMs = 2000;
	setTrigger(Message::MSG_KINECT_CALIBRATED);

}

Player::~Player() {
	// TODO Auto-generated destructor stub
}


void Player::createBodyShapes() {
	user initialUser = m_skeleton->getSkeleton();
	createHead(&initialUser.head);

	createChest(&initialUser.chest,
			&initialUser.shoulder_right,
			&initialUser.shoulder_left,
			&initialUser.pelvis_right,
			&initialUser.pelvis_left);

	createUpperArms(&initialUser.shoulder_right,
			&initialUser.shoulder_left,
			&initialUser.elbow_right,
			&initialUser.elbow_left);

	createLowerArms(&initialUser.elbow_right,
			&initialUser.elbow_left,
			&initialUser.hand_right,
			&initialUser.hand_left);

	createUpperLegs(&initialUser.pelvis_right,
			&initialUser.pelvis_left,
			&initialUser.knee_right,
			&initialUser.knee_left);

	createLowerLegs(&initialUser.knee_right,
			&initialUser.knee_left,
			&initialUser.foot_right,
			&initialUser.foot_left);


	allocateBodyPartEnum();

}

void Player::allocateBodyPartEnum(){
	ColoredWorld* coloredWorld = ColoredWorld::getInstance();

	for(int i = 0 ; i < BodyParts::BODY_COUNT; i++){
		Object* current =  coloredWorld->getObject(m_bodies[i]);
		current->setBodyPart((BodyParts::parts)i);
	}
}

void Player::createUpperArms(btVector3* rShoulder, btVector3* lShoulder,
		btVector3* rElbow, btVector3* lElbow) {

	float length;
	btTransform trans = getTransformZ(rShoulder, rElbow, &length);

	createCapsule(BodyParts::UPPER_ARM_R, &trans, 1.15, length, m_color, m_transparency);

	trans = getTransformZ(lShoulder, lElbow, &length);

	createCapsule(BodyParts::UPPER_ARM_L, &trans, 1.15, length, m_color, m_transparency);
}

void Player::createLowerArms(btVector3* rElbow, btVector3* lElbow,
		btVector3* rHand, btVector3* lHand) {

	float length;
	btTransform trans = getTransformZ(rElbow, rHand, &length);

	createCapsule(BodyParts::LOWER_ARM_R, &trans, 1.0, length, m_color, m_transparency);

	trans = getTransformZ(lElbow, lHand, &length);

	createCapsule(BodyParts::LOWER_ARM_L, &trans, 1.0, length, m_color, m_transparency);

}

void Player::createUpperLegs(btVector3* rPelvis, btVector3* lPelvis,
		btVector3* rKnee, btVector3* lKnee) {
	float length;
	btTransform trans = getTransformZ(rPelvis, rKnee, &length);

	createCapsule(BodyParts::UPPER_LEG_R, &trans, 1.3, length, m_color, m_transparency);

	trans = getTransformZ(lPelvis, lKnee, &length);

	createCapsule(BodyParts::UPPER_LEG_L, &trans, 1.3, length, m_color, m_transparency);
}

void Player::createLowerLegs(btVector3* rKnee, btVector3* lKnee,
		btVector3* rFoot, btVector3* lFoot) {
	float length;
	btTransform trans = getTransformZ(rFoot, rKnee, &length);

	createCapsule(BodyParts::LOWER_LEG_R, &trans, 1.2, length, m_color, m_transparency);

	trans = getTransformZ(lFoot, lKnee, &length);

	createCapsule(BodyParts::LOWER_LEG_L, &trans, 1.2, length, m_color, m_transparency);
}

void Player::createChest(btVector3* chest, btVector3* rShoulder,
		btVector3* lShoulder, btVector3* rPelvis, btVector3* lPelvis) {
	btVector3 upperMid;
	btVector3 lowerMid;
	btTransform trans;

	btVector3 dir = *rShoulder - *lShoulder;
	upperMid = *lShoulder + (dir * 0.5);

	dir = *rPelvis - *lPelvis;
	lowerMid = *lPelvis + (dir * 0.5);

	dir = (upperMid - lowerMid);

	float length = dir.length();

	if ((dir.angle(btVector3(0, 1, 0))) != 0) {
		trans.setRotation(*calculateRotation(new btVector3(0, 1, 0), &dir));
	}
	trans.setIdentity();
	trans.setOrigin(*chest);
	createCylinder(BodyParts::CHEST, &trans, 3, length, m_color, m_transparency);

}

void Player::createHead(btVector3* head) {
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(*head);
	createSphere(BodyParts::HEAD, &trans, 2.5, m_color, m_transparency);
}

void Player::createCapsule(int bodyIndex, btTransform* transform, float radius,
		float height, btVector3 color, btScalar transparency) {
	float mass = 0;
	m_shapes[bodyIndex]
			= new btCapsuleShape(btScalar(radius), btScalar(height));
	m_bodies[bodyIndex] = m_ownerWorld->addRigidBody(btScalar(mass), *transform,
			m_shapes[bodyIndex],COL_PLAYER, PLAYER_COL_WITH, color, transparency);

}

void Player::createCylinder(int bodyIndex, btTransform* transform,
		float radius, float height, btVector3 color, btScalar transparency) {
	float mass = 0;
	m_shapes[bodyIndex] = new btCylinderShape(
			btVector3(radius, height * 0.5, 0));
	m_bodies[bodyIndex] = m_ownerWorld->addRigidBody(btScalar(mass), *transform,
			m_shapes[bodyIndex], COL_PLAYER, PLAYER_COL_WITH, color, transparency);

}

void Player::createSphere(int bodyIndex, btTransform* transform, float radius,
		btVector3 color, btScalar transparency) {
	float mass = 0;
	m_shapes[bodyIndex] = new btSphereShape(radius);
	m_bodies[bodyIndex] = m_ownerWorld->addRigidBody(btScalar(mass), *transform,
			m_shapes[bodyIndex], COL_PLAYER, PLAYER_COL_WITH, color, transparency);
}


btQuaternion* Player::calculateRotation(btVector3* axis, btVector3* targetAxis) {
	//printf("axis: %f x %f x %f \n", axis->getX(),axis->getY(),axis->getZ());
	//printf("targetAxis: %f x %f x %f \n", targetAxis->getX(),targetAxis->getY(),targetAxis->getZ());
	btVector3 cross = targetAxis->cross(*axis);
	btScalar rotation_angle = targetAxis->angle(*axis);
	if (cross.x() == 0 && cross.y() == 0 && cross.z() == 0){
		return new btQuaternion(btVector3(1,0,0),0);
	}
//	printf("angleA: %f\n", rotation_angle);
	return new btQuaternion(cross, -rotation_angle);
}

void Player::trigger(Message msg){
	updateInit();
}



void Player::updatePlayer() {

	m_user = m_skeleton->getSkeleton();

//	m_user = computeJump(m_user);

	checkPausePressed();

	float length;
	btTransform trans;

	// Kopf
	m_bodies[BodyParts::HEAD]->getWorldTransform().setOrigin(m_user.head);

	// Chest
	btVector3 upperMid;
	btVector3 lowerMid;

	btVector3 dir = m_user.shoulder_right - m_user.shoulder_left;
	upperMid = m_user.shoulder_left + (dir * 0.5);
	dir = m_user.pelvis_right - m_user.pelvis_left;
	lowerMid = m_user.pelvis_left + (dir * 0.5);

	trans = getTransformZ(&lowerMid, &upperMid, &length);
	trans.setOrigin(m_user.chest);
	m_bodies[BodyParts::CHEST]->getWorldTransform() = trans;

	//Arms
	trans = getTransformZ(&m_user.shoulder_right, &m_user.elbow_right, &length);
	m_bodies[BodyParts::UPPER_ARM_R]->getWorldTransform() = trans;

	trans = getTransformZ(&m_user.shoulder_left, &m_user.elbow_left, &length);
	m_bodies[BodyParts::UPPER_ARM_L]->getWorldTransform() = trans;

	trans = getTransformZ(&m_user.elbow_right, &m_user.hand_right, &length);
	m_bodies[BodyParts::LOWER_ARM_R]->getWorldTransform() = trans;

	trans = getTransformZ(&m_user.elbow_left, &m_user.hand_left, &length);
	m_bodies[BodyParts::LOWER_ARM_L]->getWorldTransform() = trans;


	//Legs
	trans = getTransformZ(&m_user.pelvis_right, &m_user.knee_right, &length);
	m_bodies[BodyParts::UPPER_LEG_R]->getWorldTransform() = trans;

	trans = getTransformZ(&m_user.pelvis_left, &m_user.knee_left, &length);
	m_bodies[BodyParts::UPPER_LEG_L]->getWorldTransform() = trans;

	trans = getTransformZ(&m_user.foot_right, &m_user.knee_right, &length);
	m_bodies[BodyParts::LOWER_LEG_R]->getWorldTransform() = trans;

	trans = getTransformZ(&m_user.foot_left, &m_user.knee_left, &length);
	m_bodies[BodyParts::LOWER_LEG_L]->getWorldTransform() = trans;

}

#define SET_JUMP(part)										\
		u.part.setY(u.part.getY() + jumpHeight*jumpFactor);

user Player::computeJump(user u){
	btVector3 leftFoot = u.foot_left, rightFoot = u.foot_right;
	btScalar jumpHeight = (leftFoot.y() < rightFoot.y()) ? leftFoot.y() : rightFoot.y();

	// normalize the height
	if(jumpHeight <= 1){
		jumpHeight = 0;
	}
	btScalar jumpFactor = 6.4;

//	printf("jumpheight: %f\n",jumpHeight);

	SET_JUMP(head);
	SET_JUMP(shoulder_left);
	SET_JUMP(shoulder_right);
	SET_JUMP(elbow_left);
	SET_JUMP(elbow_right);
	SET_JUMP(hand_left);
	SET_JUMP(hand_right);
	SET_JUMP(chest);
	SET_JUMP(pelvis_left);
	SET_JUMP(pelvis_right);
	SET_JUMP(knee_left);
	SET_JUMP(knee_right);
	SET_JUMP(foot_left);
	SET_JUMP(foot_right);

	return u;
}

void Player::checkPausePressed(){
	bool pause = false;
	float thresh = 2.5;

	btScalar distance = m_user.foot_left.distance(m_user.hand_right);
	if(distance < thresh){
		pause = true;
	}

	distance = m_user.foot_right.distance(m_user.hand_left);
	if(distance < thresh){
		pause = true;
	}

	if(m_pauseClock != 0){

		if(pause && (m_pauseClock->getTimeMilliseconds() < m_pauseTimeThreshMs)){
			pause = false;
		}

	}

	if(pause && !m_justPushedPause){
		Message msg;
		msg.messageType = Message::MSG_PAUSE_GAME;
		sendMessage(msg);
//		printf("pause pushed\n");
		m_justPushedPause = true;
		m_pauseClock = new btClock();

	} else {
		if(m_justPushedPause){
			m_justPushedPause = false;
		}
	}
}

btTransform Player::getTransformZ(btVector3* origin, btVector3* target,
								  float* side_length) {
	btTransform trans;

	btVector3 lengthV = *target - *origin;
	float length = lengthV.length();
	*side_length = length;
	btVector3 mid = *origin + (lengthV * 0.5);
	trans.setIdentity();

	if (lengthV.angle(btVector3(0, 1, 0)) != 0 && lengthV.angle(btVector3(0, 1, 0)) != 180) {
		trans.setRotation(*calculateRotation(new btVector3(0, 1, 0), &lengthV));
	}
	trans.setOrigin(mid);
	return trans;
}

void Player::updateInit(){
	user u = m_skeleton->getSkeleton();

	btVector3 lengthV;
	float length;

	for(int i=0; i < BodyParts::BODY_COUNT; i++){
		delete m_shapes[i];
	}

	m_bodies[BodyParts::HEAD]->setCollisionShape(new btSphereShape(2.5));
	btVector3 upperMid;
	btVector3 lowerMid;

	btVector3 dir = u.shoulder_right - u.shoulder_left;
	upperMid = u.shoulder_left + (dir * 0.5);

	dir = u.pelvis_right - u.pelvis_left;
	lowerMid = u.pelvis_left + (dir * 0.5);
	dir = (upperMid - lowerMid);

	length = dir.length();
	m_bodies[BodyParts::CHEST]->setCollisionShape(new btCylinderShape(btVector3(3,length*0.5,0)));

	lengthV = u.hand_right - u.elbow_right;
	length = lengthV.length();

	m_bodies[BodyParts::LOWER_ARM_R]->setCollisionShape(new btCapsuleShape(1,length));

	lengthV = u.hand_left - u.elbow_left;
	length = lengthV.length();

	m_bodies[BodyParts::LOWER_ARM_L]->setCollisionShape(new btCapsuleShape(1,length));

	lengthV = u.elbow_right - u.shoulder_right;
	length = lengthV.length();

	m_bodies[BodyParts::UPPER_ARM_R]->setCollisionShape(new btCapsuleShape(1.15,length));

	lengthV = u.elbow_left - u.shoulder_left;
	length = lengthV.length();

	m_bodies[BodyParts::UPPER_ARM_L]->setCollisionShape(new btCapsuleShape(1.15,length));

	//LEGS

	lengthV = u.knee_right - u.foot_right;
	length = lengthV.length();

	m_bodies[BodyParts::LOWER_LEG_R]->setCollisionShape(new btCapsuleShape(1.2,length));

	lengthV = u.knee_left - u.foot_left;
	length = lengthV.length();

	m_bodies[BodyParts::LOWER_LEG_L]->setCollisionShape(new btCapsuleShape(1.2,length));

	lengthV = u.pelvis_right - u.knee_right;
	length = lengthV.length();

	m_bodies[BodyParts::UPPER_LEG_R]->setCollisionShape(new btCapsuleShape(1.3,length));

	lengthV = u.pelvis_left - u.knee_left;
	length = lengthV.length();

	m_bodies[BodyParts::UPPER_LEG_L]->setCollisionShape(new btCapsuleShape(1.3,length));
}

BodyParts::parts Player::getBodyPart(btCollisionShape* body){
	BodyParts::parts result = BodyParts::BODY_COUNT;

	for(int i = 0; i < BodyParts::BODY_COUNT; i++){
		if(m_shapes[i] == body){
			result = (BodyParts::parts)i;
		}
	}

	return result;
}

float Player::getAdaptedRadius(){

}
