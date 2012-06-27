/*
 * Canon.h
 *
 *  Created on: Apr 27, 2011
 *      Author: Tobias Baumgartner
 */

#ifndef CANON_H_
#define CANON_H_

#include "btBulletDynamicsCommon.h"
#include "ColoredWorld.h"
#include "Skeleton.h"
#include "FlyingObject.h"
#include "ShapeFactory.h"
#include "LevelHandler.h"

#include "GameConfigurations/ArcadeConfiguration.h"

class Cannon : public SignalUser{
public:
	Cannon(ColoredWorld* dynamicsWorld);
	virtual ~Cannon();
	void process();
	void set_position(btVector3 position);
	void set_target(btVector3 target);
	void setShootingDirection(btVector3 direction);
    float getFrequency() const;
    void setFrequency(float m_frequency);
    float getSpeed() const;
    void setSpeed(float m_speed);
    bool getShootRandom() const;
    void setShootRandom(bool m_shootRandom);

    void setConfiguration(ArcadeConfiguration* config);
    void updateLevelData(int level);

    void trigger(Message msg);

    //TEST
    void setBulletShape(btCollisionShape* shape);

private:
    void shoot(const btVector3 & destination, FlyingObject::FlyingObjectType type, btVector3 angularVec);
    void shootRandom();
    btScalar get_time_delta_in_ms();
    void update_transforms();
    void update_canon();
    void draw_canon();
    void create_basement();
    void create_muzzle();

private:
    btVector3 m_position;
	btVector3 m_bullet_start;
	float m_speed;
	ColoredWorld* m_coloredWorld;
	btCollisionShape* m_bulletShape;
	btScalar m_defaultContactProcessingThreshold;
	btVector3 m_aim_target;
	btVector3 m_muzzle_shape;
	btClock m_clock;
	btScalar m_last_time;
	btScalar m_time_since_last_shot;

	float m_frequency;

	btRigidBody* m_basement;
	btRigidBody* m_muzzle;
	btTransform m_basement_transform;
	btTransform m_muzzle_transform;

	bool m_shootRandom;

	Skeleton* m_skeleton;

	btRigidBody* m_lastBullet;

	ShapeFactory m_shapeFac;

	ArcadeConfiguration* m_config;

	LevelHandler* m_levelHandler;

};

#endif /* CANON_H_ */
