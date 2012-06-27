/*
 * Canon.cpp
 *
 *  Created on: Apr 27, 2011
 *      Author: Tobias Baumgartner
 */

#include "Cannon.h"
#include "CollisionClasses.h"

using namespace std;

Cannon::Cannon(ColoredWorld* coloredWorld) {
	m_coloredWorld = coloredWorld;
	m_position = btVector3(50,1,-10);
	m_bullet_start = m_position;
	m_aim_target = btVector3(0,25,0);
	m_speed = 50.f;
//	m_bulletShape = new btCapsuleShape(2.f,1.f);
	m_bulletShape = m_shapeFac.getShape(m_shapeFac.SPIKEDBALL_SHAPE);
//	m_bulletShape = new btBoxShape(btVector3(1,1,5));
	m_defaultContactProcessingThreshold = BT_LARGE_FLOAT;
	m_last_time = m_clock.getTimeMilliseconds();
	m_time_since_last_shot = 0.f;
	m_frequency = 1;
	m_shootRandom = true;
	m_lastBullet = 0;
	draw_canon();
	m_config = 0;

	m_levelHandler = LevelHandler::getInstance();

	m_skeleton = Skeleton::getInstance();

	setTrigger(Message::MSG_START_FLIGHT);

}

void Cannon::trigger(Message msg){
	switch(msg.messageType){
	case Message::MSG_START_FLIGHT:
		shoot(btVector3(5,50,0),FlyingObject::FOM_BAD,btVector3(5,0,9));
		break;
	}
}

Cannon::~Cannon() {
	delete m_bulletShape;
}

void Cannon::setBulletShape(btCollisionShape* shape){
	m_bulletShape = shape;
}

void Cannon::process(){
	float ms = 1/m_frequency*1000;

	if(m_time_since_last_shot > ms){
		//shoot(m_aim_target);
		if(m_shootRandom){
			shootRandom();
		} else {
			int random = (rand()*10)%3;
			random = 2;
			int color;
			switch(random){
			case 0:
				color = FlyingObject::FOM_BAD;
				break;
			case 1:
				color = FlyingObject::FOM_GOOD;
				break;
			case 2:
				color = FlyingObject::FOM_REFLECT;
				break;

			}
			shoot(m_aim_target, (FlyingObject::FlyingObjectType)color, btVector3(0,0,0));
		}
		m_time_since_last_shot = 0.f;


		Message msg;
		msg.messageType = Message::MSG_SHOT_OBJECT;
		sendMessage(msg);
	} else {
		m_time_since_last_shot += this->get_time_delta_in_ms();
	}
}

void Cannon::set_position(btVector3 position){
	m_position = position;
	update_canon();
}

void Cannon::set_target(btVector3 target){
	m_aim_target = target;
	update_canon();
}

void Cannon::setShootingDirection(btVector3 direction){
	m_aim_target = m_position + direction*10;
	update_canon();
}

void Cannon::update_transforms(){

	m_basement_transform.setIdentity();
	m_basement_transform.setOrigin(m_position);

	m_muzzle_transform.setIdentity();
	m_muzzle_shape = btVector3(2.5f,.2f,.2f);
	m_muzzle_transform.setOrigin(m_position /*+ m_muzzle_shape/2*/ + btVector3(0.f,2.f,0.f));

	btVector3 shoot_direction = m_aim_target - m_muzzle_transform.getOrigin();
	btVector3 orientation_of_muzzle = btVector3(1.f,0.f,0.f);
	btVector3 rotation_axis = shoot_direction.cross(orientation_of_muzzle);
	btScalar rotation_angle = shoot_direction.angle(orientation_of_muzzle);
	m_muzzle_transform.setRotation(btQuaternion(rotation_axis,-rotation_angle));

	m_bullet_start = m_muzzle_transform.getOrigin() + shoot_direction.normalize()*m_muzzle_shape.length()*2;

}

void Cannon::draw_canon(){
	update_transforms();
	create_muzzle();
	create_basement();
}

void Cannon::create_basement(){
	btCompoundShape* basement = new btCompoundShape();//static scenario

	btCollisionShape* stage1 = new btCylinderShape(btVector3(3,.35,0));
	btCollisionShape* stage2 = new btCylinderShape(btVector3(2.5,.25,0));
	btCollisionShape* stage3 = new btCylinderShape(btVector3(2,.25,0));
	btCollisionShape* stage4 = new btCylinderShape(btVector3(.6,1,0));
	btCollisionShape* stage5 = new btCapsuleShape(.6,.5);
	btTransform	basement_transform;
	basement_transform.setIdentity();

	basement_transform.setOrigin(btVector3(0,-.3,0));
	basement->addChildShape(basement_transform,stage1);
	basement_transform.setOrigin(btVector3(0,.4,0));
	basement->addChildShape(basement_transform,stage2);
	basement_transform.setOrigin(btVector3(0,1,0));
	basement->addChildShape(basement_transform,stage3);
	basement_transform.setOrigin(btVector3(0,2.25,0));
	basement->addChildShape(basement_transform,stage4);
	basement_transform.setOrigin(btVector3(0,2.7,0));
	basement->addChildShape(basement_transform,stage5);


//	basement_transform.setOrigin(btVector3(0,0,0));

	m_basement = m_coloredWorld->addRigidBody(0, m_basement_transform,basement,COL_STRUCTURES,STRUCTURES_COL_WITH);
}

void Cannon::create_muzzle(){
	m_muzzle = m_coloredWorld->addRigidBody(0, m_muzzle_transform,
			new btBoxShape(m_muzzle_shape),COL_STRUCTURES,STRUCTURES_COL_WITH);
//	m_muzzle->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

void Cannon::update_canon(){
	update_transforms();
	m_muzzle->getWorldTransform() = m_muzzle_transform;
	m_basement->getWorldTransform() = m_basement_transform;
}

void Cannon::shoot(const btVector3& destination, FlyingObject::FlyingObjectType type, btVector3 angularVec) {

	float mass = 2.f;
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(m_bullet_start);
	btVector3 startPosition = startTransform.getOrigin();

	btRigidBody* body = m_coloredWorld->addFlyingObject(mass, startTransform,
			m_bulletShape,type);

	body->setAngularVelocity(angularVec);
	body->setLinearFactor(btVector3(1, 1, 1));
	//body->setRestitution(1);

	btVector3 linVel(destination[0] - startPosition[0], destination[1] - startPosition[1],
			destination[2] - startPosition[2]);
	linVel.normalize();
	linVel *= m_speed;

	body->getWorldTransform().setOrigin(startPosition);
	body->getWorldTransform().setRotation(btQuaternion(0, 0, 0, 1));
	body->setLinearVelocity(linVel);
//	body->setAngularVelocity(btVector3(0, 0, 0));
	body->setGravity(btVector3(0,-10,0));
	body->setCcdMotionThreshold(0.5);
	body->setCcdSweptSphereRadius(0.1f);
	m_lastBullet = body;
}

void Cannon::shootRandom() {

	float rando = m_skeleton->getRandomFloat();
	int offset = (int)(rando*100);
	btVector3 destination;
	destination.setY(8 + (offset%5)*4.5);
	rando = m_skeleton->getRandomFloat();
	offset = (int)(rando*100);
	destination.setX((offset%4)*5 - 10);
	destination.setZ(0);
	destination = destination + m_aim_target;

	ShapeProbability sProb = *(m_config->getShapeByValue((rand()%100 + 1),m_levelHandler->getCurrentLevel()));
	string setAngular = "Box";
	string setAngular2 = "CylinderY";
	string setAngular3 = "Compound";
	string shapeName = sProb.getShape()->getName();
	btVector3 angularVec;
	if(setAngular.compare(shapeName) == 0 || setAngular2.compare(shapeName) == 0 || setAngular3.compare(shapeName) == 0){
		angularVec = btVector3(rand()%5,rand()%5,rand()%5);
	}
	else{angularVec = btVector3(0,0,0);}
	m_bulletShape = sProb.getShape();
	int randomType = (rand()%100 +1);
	int stackCount = 0;
	int color = 0;

	stackCount += (int)sProb.getProbGood();
	if(randomType <= stackCount){
		color = 1;
	}
	else{
		stackCount += (int)sProb.getProbBad();
		if(randomType <= stackCount){
			color = 0;
		}
		else{
			stackCount += (int)sProb.getProbReflect();
			if(randomType <= stackCount){
				color = 2;
			}
		}
	}


	switch (color){
	case 0:
		shoot(destination, FlyingObject::FOM_BAD, angularVec);
		break;
	case 1:
		shoot(destination, FlyingObject::FOM_GOOD, angularVec);
		break;
	case 2:
		shoot(destination, FlyingObject::FOM_REFLECT, angularVec);
		break;
	}
}

void Cannon::setConfiguration(ArcadeConfiguration* config){
	m_config = config;
}

void Cannon::updateLevelData(int level){
	m_speed = m_config->getShootSpeed(level);
	m_frequency = m_config->getShootFrequency(level);
	m_position = m_config->getCanonPosition(level);
	m_aim_target = m_config->getShootingTarget(level);
}


btScalar Cannon::get_time_delta_in_ms(){
	btScalar this_time = m_clock.getTimeMilliseconds();
	btScalar time_difference = this_time - m_last_time;
	m_last_time = this_time;
	return time_difference;
}

float Cannon::getFrequency() const
{
    return m_frequency;
}

void Cannon::setFrequency(float m_frequency)
{
    this->m_frequency = m_frequency;
}

float Cannon::getSpeed() const
{
    return m_speed;
}

void Cannon::setSpeed(float m_speed)
{
    this->m_speed = m_speed;
}

bool Cannon::getShootRandom() const
{
    return m_shootRandom;
}

void Cannon::setShootRandom(bool m_shootRandom)
{
    this->m_shootRandom = m_shootRandom;
}






