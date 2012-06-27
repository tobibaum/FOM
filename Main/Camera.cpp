/*
 * Camera.cpp
 *
 *  Created on: May 31, 2011
 *      Author: Tobi
 */

#include "Camera.h"

Camera::Camera(Field* field) {
	m_field = field;
	m_coloredWorld = ColoredWorld::getInstance();
	setTrigger(Message::MSG_CAMERA_INITIALIZED);
	setTrigger(Message::MSG_COL_PLAYER_FLY);
	initValues();
}

void Camera::initValues(){
	m_bPositionChanged = false;
	m_bRotateChanged = false;
	m_bSlopeChanged = false;
	m_bTargetChanged = false;
	m_bUpVectorChanged = false;

	m_rad2deg = 0.01745329251994329547;
	m_position = m_field->getCameraPosition();
	m_target = m_field->getCameraTargetPosition();
	m_direction = m_target - m_position;
	m_upVector = btVector3(0,1,0);
	m_updateCamera = false;
	m_flyCamera = false;
	m_shakeCamera = false;
	m_computeFlyParameters = false;
	m_shakeStart = 0;

//	float pixProSecond = ;

	m_flySpeed = 0.08;


	btVector3 forward(0,0,1);
	btVector3 side = m_target-m_position;
	side.setY(0);
	if(m_position.x() < 0){
		m_rotate = forward.angle(side)/m_rad2deg;
	} else {
		m_rotate = forward.angle(side)/m_rad2deg;
	}

	btVector3 down(0,-1,0);
	side = m_target-m_position;
	side.setX(0);
	m_slope = 90-down.angle(side)/m_rad2deg;
}

Camera::~Camera() {

}

/**
* This is the main loop for the camera
*/
void Camera::process(btScalar ms){
	if(m_computeFlyParameters)
		computeFlyParameters();

	if(m_flyCamera)
		processCameraFlight(ms);

	if(m_shakeCamera)
		processCameraShake();


	if(m_updateCamera)
		processCameraUpdate();
}

void Camera::computeFlyParameters(){

	m_deltaPosition = (m_position - m_basePosition);
	m_deltaTarget = (m_target - m_baseTarget);
	m_endPosition = m_position;
	m_endTarget = m_target;
	m_position = m_basePosition;
	m_target = m_baseTarget;
	m_computeFlyParameters = false;

	m_cumulatedFlightTime = 0;
	btScalar significantDelta = (m_deltaPosition.length() > m_deltaTarget.length()) ?
			m_deltaPosition.length() : m_deltaTarget.length();

	m_overallFlightTime = significantDelta / m_flySpeed;

}

void Camera::processCameraFlight(btScalar ms){
	m_cumulatedFlightTime += ms;


	m_position += m_deltaPosition*ms/m_overallFlightTime;
	m_target += m_deltaTarget*ms/m_overallFlightTime;

	m_updateCamera = true;
//	m_cumulatedFlightTime = 0;


//	if((m_target - m_endTarget).length() < m_deltaTarget.length()){
//		m_deltaTarget = btVector3(0,0,0);
//	}
//
//	if((m_position - m_endPosition).length() < m_deltaPosition.length()){
//		m_deltaPosition = btVector3(0,0,0);
//	}

	if(m_cumulatedFlightTime > m_overallFlightTime){
		m_flyCamera = false;
		m_updateCamera = false;
	}
}

void Camera::processCameraShake(){
	int rand = random();
	int rand1 = rand*3%10;
	int rand2 = rand*4%10;

	if(m_shakeStart == 0){
		m_shakeStart = new btClock();
	}

	if(m_shakeStart->getTimeMilliseconds() > 500){
		delete m_shakeStart;
		m_shakeStart = 0;
		m_shakeCamera = false;
	} else {
		m_rotateOffset = rand1;
		m_slopeOffset = rand2;
	}

	updateCamera();
}

void Camera::processCameraUpdate(){
	m_field->setCameraDistance((m_position-m_target).length());
	btVector3 forward(0,0,1);
	btVector3 side = m_target-m_position;
	side.setY(0);

	if(m_position.x() < 0){
		m_field->setAzi(-forward.angle(side)/m_rad2deg + m_rotateOffset);
	} else {
		m_field->setAzi(forward.angle(side)/m_rad2deg + m_rotateOffset);
	}

	btVector3 down(0,-1,0);
	side = m_target-m_position;
	m_field->setEle(90-down.angle(side)/m_rad2deg + m_slopeOffset);

	m_slopeOffset = m_rotateOffset = 0;
	m_field->setCameraTargetPosition(m_target);
	m_field->updateCamera();
	m_updateCamera = false;

}

void Camera::updateCamera(){
	m_updateCamera = true;
}
void Camera::flyCameraTo(){
	m_computeFlyParameters = true;
	m_basePosition = m_position;
	m_baseTarget = m_target;
	m_flyCamera = true;
}

void Camera::shakeCamera(){
	m_shakeCamera = true;
}

void Camera::setColorFilter(btVector3 color){
	btTransform trans;
	trans.setIdentity();
	btVector3 pos = m_field->getCameraPosition(), tar = m_field->getCameraTargetPosition();
	trans.setOrigin(pos + (pos-tar).normalized() - btVector3(0,0,5));
	trans.setRotation(btQuaternion(btVector3(1,0,0),-m_slope));
//	trans.setRotation();

//	trans.setOrigin(btVector3(0,0,0));
	m_colorFilter = m_coloredWorld->addRigidBody(0,trans,new btBoxShape(btVector3(1000,0.01,1000)),COL_STRUCTURES,0,color,0.6);
}

void Camera::trigger(Message msg){

	switch(msg.messageType){
	case Message::MSG_CAMERA_INITIALIZED:
		initValues();
		break;

	case Message::MSG_COL_PLAYER_FLY:
		switch(msg.additionalInfo){
		case FlyingObject::FOM_BAD:
			shakeCamera();
			setColorFilter(btVector3(1,0,0));
			Cleaner::getInstance()->deleteBody(m_colorFilter, 500);
			break;
		case FlyingObject::FOM_GOOD:
			setColorFilter(btVector3(0,1,0));
			Cleaner::getInstance()->deleteBody(m_colorFilter, 75);
			break;

		case FlyingObject::FOM_REFLECT:
//			setColorFilter(btVector3(0,0,.5));
//			Cleaner::getInstance()->deleteBody(m_colorFilter, 75);
			break;
		}
		break;
	}
}

btVector3 Camera::getPosition()
{
    return m_position;
}

btVector3 Camera::getTarget()
{
	m_target = m_field->getCameraTargetPosition();
    return m_target;
}

btVector3 Camera::getDirection() const
{
    return m_target - m_position;
}

void Camera::setPosition(btVector3 position)
{
	m_bPositionChanged = true;
    this->m_position = position;
    updateCamera();
}

void Camera::setTarget(btVector3 target)
{
	m_bTargetChanged = true;
    this->m_target = target;
//    m_field->setCameraTargetPosition(target);

    updateCamera();
}

void Camera::setDirection(btVector3 direction)
{
	m_bTargetChanged = true;
	m_target = m_position + 10 * direction;
	m_direction = direction;
	updateCamera();
}

btVector3 Camera::getUpVector() const
{
    return m_upVector;
}

void Camera::setUpVector(btVector3 m_upVector)
{
	m_bUpVectorChanged = true;
	this->m_upVector = m_upVector;
    updateCamera();
}

btScalar Camera::getRotate() const
{
    return m_rotate;
}

btScalar Camera::getSlope() const
{
    return m_slope;
}

void Camera::setRotate(btScalar m_rotate)
{
	m_bRotateChanged = true;
    this->m_rotate = m_rotate;
    updateCamera();
}

void Camera::setSlope(btScalar m_slope)
{
	m_bSlopeChanged = true;
    this->m_slope = m_slope;
    updateCamera();
}






