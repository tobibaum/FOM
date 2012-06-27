/*
 * HandTracker.h
 *
 *  Created on: Jun 21, 2011
 *      Author: hendrik
 */

#ifndef HANDTRACKER_H_
#define HANDTRACKER_H_

#define SPEED_BOOST 7

#include "Skeleton.h"
#include "btBulletDynamicsCommon.h"


class HandTracker {

public:
	enum trackedParts{
		TRACK_R_HAND = 0,
		TRACK_L_HAND,
		TRACK_R_FOOT,
		TRACK_L_FOOT
	};

private:
	HandTracker();
	virtual ~HandTracker();

public:
	static HandTracker* getInstance();

	void trackHand();
	void process();

	btScalar getSpeed(trackedParts part);
	btVector3* getMoveDirection(trackedParts part);

private:
	static HandTracker* m_Instance;

	btClock m_trackClock;
	btScalar m_tracktime;
	btScalar m_timeSinceLastTrack;
	btScalar m_lastTrackTime;

	btVector3 m_handPositionOld;
	btScalar m_handDistance;
	btVector3 m_moveDirection;

	btVector3 m_LhandPositionOld;
	btScalar m_LhandDistance;
	btVector3 m_LhandMoveDirection;

	btVector3 m_LfootPositionOld;
	btScalar m_LfootDistance;
	btVector3 m_LfootMoveDirection;

	btVector3 m_RfootPositionOld;
	btScalar m_RfootDistance;
	btVector3 m_RfootMoveDirection;

	Skeleton* m_skeleton;
};

#endif /* HANDTRACKER_H_ */
