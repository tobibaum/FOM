/*
 * Skeleton.h
 *
 *  Created on: May 3, 2011
 *      Author: Tobi
 */

#ifndef SKELETON_H_
#define SKELETON_H_

#define KINECT_DEBUG
#ifndef KINECT_DEBUG
#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
#else
#include "/usr/include/ni/XnOpenNI.h"
#include "/usr/include/ni/XnCodecIDs.h"
#include "/usr/include/ni/XnCppWrapper.h"
#endif

#include <list>
#include <stdio.h>


#include "GlutStuff.h"
#include "SignalUser.h"

#include "btBulletDynamicsCommon.h"
#define SAMPLE_XML_PATH "player.xml"

struct user{
	btVector3 head ;
	btVector3 chest;
	btVector3 elbow_left;
	btVector3 elbow_right;
	btVector3 hand_left;
	btVector3 hand_right;
	btVector3 shoulder_left;
	btVector3 shoulder_right;
	btVector3 pelvis_left;
	btVector3 pelvis_right;
	btVector3 knee_left;
	btVector3 knee_right;
	btVector3 foot_left;
	btVector3 foot_right;
};

class Skeleton : public SignalUser{
private:
	Skeleton();
	virtual ~Skeleton();

public:
    static Skeleton *getInstance();
    static void destroy();
    user getSkeleton();

    float getRandomFloat();
    void process();
    std::list<std::string> getDescriptionString();

    void trigger(Message msg);

    XnStatus init_kinect();

    void setCallbacks();

private:

    int getConfidence();
private:
    static Skeleton* m_theInstance;
	user m_current_user;
	float last_rand;
	bool m_calibrated;
	bool m_sentConfidenceMessage;

	bool m_kinect_stopped;

	std::list<std::string> m_descriptionString;
};

#endif /* SKELETON_H_ */
