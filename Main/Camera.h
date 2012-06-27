/*
 * Camera.h
 *
 *  Created on: May 31, 2011
 *      Author: Tobi
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Field.h"
#include "Cleaner.h"

class Field;

class Camera : public SignalUser{
public:
	Camera(Field* field);
	virtual ~Camera();
    btVector3 getPosition() ;
    btVector3 getTarget() ;
    btVector3 getDirection() const;
    void setPosition(btVector3 position);
    void setTarget(btVector3 target);
    void setDirection(btVector3 direction);
    btVector3 getUpVector() const;
    void setUpVector(btVector3 m_upVector);


    void process(btScalar ms);

    void flyCameraTo();
    void setColorFilter(btVector3 color);
    void shakeCamera();

    btScalar getRotate() const;
    btScalar getSlope() const;
    void setRotate(btScalar m_rotate);
    void setSlope(btScalar m_slope);

private: /* methods */
    void initValues();
    void updateCamera();
    void computeFlyParameters();
    void processCameraFlight(btScalar ms);
    void processCameraShake();
    void processCameraUpdate();
    void trigger(Message msg);

private: /* members */


    btScalar m_rad2deg;

	btVector3 m_position;
	btVector3 m_target;
	btVector3 m_direction;
	btVector3 m_upVector;

	btVector3 m_deltaTarget;
	btVector3 m_deltaPosition;
	btVector3 m_basePosition;
	btVector3 m_baseTarget;
	btVector3 m_endPosition;
	btVector3 m_endTarget;

	btScalar m_slope;
	btScalar m_rotate;
	btScalar m_slopeOffset;
	btScalar m_rotateOffset;
	btScalar m_cumulatedFlightTime;
	btScalar m_overallFlightTime;
	btScalar m_flySpeed;

	btClock* m_shakeStart;
	btClock* m_flightClock;

	btRigidBody* m_colorFilter;

	ColoredWorld* m_coloredWorld;

	bool m_bPositionChanged;
	bool m_bTargetChanged;
	bool m_bUpVectorChanged;
	bool m_bRotateChanged;
	bool m_bSlopeChanged;

	bool m_flyCamera;
	bool m_shakeCamera;
	bool m_computeFlyParameters;
	bool m_updateCamera;
	Field* m_field;
};

#endif /* CAMERA_H_ */
