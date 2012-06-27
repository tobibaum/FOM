/*
 * HandTracker.cpp
 *
 *  Created on: Jun 21, 2011
 *      Author: hendrik
 */

#include "HandTracker.h"

HandTracker* HandTracker::m_Instance = 0;

HandTracker::HandTracker() {
	m_skeleton = Skeleton::getInstance();

	m_handPositionOld = btVector3(0,0,0);
	m_timeSinceLastTrack = 0.f;
	m_tracktime = 50.f;
	m_lastTrackTime = m_trackClock.getTimeMilliseconds();
}

HandTracker::~HandTracker() {
	// TODO Auto-generated destructor stub
}

HandTracker* HandTracker::getInstance(){
	if(!m_Instance){
		m_Instance = new HandTracker();
	}

	return m_Instance;
}

void HandTracker::trackHand(){
		btScalar ms = m_tracktime;
		if(m_timeSinceLastTrack > ms){
			//RIGHT-HAND
			user usr = m_skeleton->getSkeleton();
			btVector3 handPos = usr.hand_right;
			m_moveDirection = handPos - m_handPositionOld;
			m_handDistance = (m_handPositionOld - handPos).length();
			//	delete m_handPositionOld;
			m_handPositionOld.setX(handPos.getX());
			m_handPositionOld.setY(handPos.getY());
			m_handPositionOld.setZ(handPos.getZ());

			//LEFT HAND

			btVector3 LhandPos = usr.hand_left;
			m_LhandMoveDirection = LhandPos - m_LhandPositionOld;
			m_LhandDistance = (m_LhandPositionOld - LhandPos).length();
			//	delete m_handPositionOld;
			m_LhandPositionOld.setX(LhandPos.getX());
			m_LhandPositionOld.setY(LhandPos.getY());
			m_LhandPositionOld.setZ(LhandPos.getZ());

			//RIGHT FOOT

			btVector3 RfootPos = usr.foot_right;
			m_RfootMoveDirection = RfootPos - m_RfootPositionOld;
			m_RfootDistance = (m_RfootPositionOld - RfootPos).length();
			//	delete m_handPositionOld;
			m_RfootPositionOld.setX(RfootPos.getX());
			m_RfootPositionOld.setY(RfootPos.getY());
			m_RfootPositionOld.setZ(RfootPos.getZ());

			//LEFT FOOT

			btVector3 LfootPos = usr.foot_left;
			m_LfootMoveDirection = LfootPos - m_LfootPositionOld;
			m_LfootDistance = (m_LfootPositionOld - LfootPos).length();
			//	delete m_handPositionOld;
			m_LfootPositionOld.setX(LfootPos.getX());
			m_LfootPositionOld.setY(LfootPos.getY());
			m_LfootPositionOld.setZ(LfootPos.getZ());

			m_timeSinceLastTrack = 0.f;
		}
		else{
			btScalar this_time = m_trackClock.getTimeMilliseconds();
			m_timeSinceLastTrack += this_time - m_lastTrackTime;
			m_lastTrackTime = this_time;
		}
}

btScalar HandTracker::getSpeed(trackedParts part){
	switch(part){
	case TRACK_R_HAND : return m_handDistance*SPEED_BOOST;
		break;
	case TRACK_L_HAND : return m_LhandDistance*SPEED_BOOST;
		break;
	case TRACK_R_FOOT : return m_RfootDistance*SPEED_BOOST;
		break;
	case TRACK_L_FOOT : return m_LfootDistance*SPEED_BOOST;
		break;
	}

}

btVector3* HandTracker::getMoveDirection(trackedParts part){
	switch(part){
	case TRACK_R_HAND : return &m_moveDirection;
		break;
	case TRACK_L_HAND : return &m_LhandMoveDirection;
		break;
	case TRACK_R_FOOT : return &m_RfootMoveDirection;
		break;
	case TRACK_L_FOOT : return &m_LfootMoveDirection;
		break;
	}
}

void HandTracker::process(){
	trackHand();
}
