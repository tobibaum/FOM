/*
 * Skeleton.cpp
 *
 *  Created on: May 3, 2011
 *      Author: Tobi
 */

#include "Skeleton.h"
#include <algorithm>

#define WANTED_HEIGHT 23
#define CONFIDENCE_TARGET_VALUE 14


xn::Context g_Context;
xn::DepthGenerator g_DepthGenerator;
xn::UserGenerator g_UserGenerator;
user g_user;
btVector3 g_rotation_axis;
btScalar g_rotation_angle;
btVector3 g_translation = btVector3(0,0,0);
btScalar g_scale_factor = 1;

XnUserID g_nPlayer = 0;
XnBool g_bCalibrated = FALSE;
bool g_bStartedKinect = false;
bool g_sentConfidenceMessage = false;
Skeleton* Skeleton::m_theInstance = 0;

std::list<std::string> g_descriptionString;

void setPlayerJoints(XnUserID user_id);
void FindPlayer();
void XN_CALLBACK_TYPE NewUser(xn::UserGenerator& generator, XnUserID user, void* pCookie);
void XN_CALLBACK_TYPE LostUser(xn::UserGenerator& generator, XnUserID user, void* pCookie);
void XN_CALLBACK_TYPE CalibrationStarted(xn::SkeletonCapability& skeleton, XnUserID user, void* cxt);
void XN_CALLBACK_TYPE PoseDetected(xn::PoseDetectionCapability& pose, const XnChar* strPose, XnUserID user, void* cxt);
void XN_CALLBACK_TYPE CalibrationEnded(xn::SkeletonCapability& skeleton, XnUserID user, XnBool bSuccess, void* cxt);


#define START_CAPTURE_CHECK_RC(rc, what)									\
	if (nRetVal != XN_STATUS_OK)											\
{																			\
	printf("Failed to %s: %s\n", what, xnGetStatusString(rc));				\
	StopCapture();															\
	return ;																\
}

#define CHECK_RC(rc, what)                                  \
	printf("%s: %s\n", what, xnGetStatusString(rc));		\
	if (rc != XN_STATUS_OK)                                 \
	{                                                       \
			return rc;                                      \
	}


Skeleton* Skeleton::getInstance(){
	if(!m_theInstance){
		m_theInstance = new Skeleton();
	}

	return m_theInstance;
}

void Skeleton::destroy(){
	if(m_theInstance){
		delete m_theInstance;
	}
	m_theInstance = 0;
}

Skeleton::Skeleton() {
	last_rand = 0;
	m_calibrated = false;
	m_descriptionString.push_back("Please step in front");
	m_descriptionString.push_back("of the Kinect.");

	setTrigger(Message::MSG_STOP_KINECT);
}

Skeleton::~Skeleton() {

}

void Skeleton::trigger(Message msg){
	switch(msg.messageType){
	case Message::MSG_STOP_KINECT:
		// we need to halt the kinect. redo calibration!

		g_UserGenerator.GetSkeletonCap().Reset(g_nPlayer);
		g_UserGenerator.GetSkeletonCap().AbortCalibration(g_nPlayer);
		g_UserGenerator.GetSkeletonCap().RequestCalibration(g_nPlayer, false);
//		g_UserGenerator.GetPoseDetectionCap().StartPoseDetection("Psi", g_nPlayer);
		g_nPlayer = 0;
		g_bCalibrated = false;


		XnCallbackHandle hUserCBs, hCalibrationCBs, hPoseCBs;
		g_UserGenerator.RegisterUserCallbacks(NewUser, LostUser, NULL, hUserCBs);
		g_UserGenerator.GetSkeletonCap().RegisterCalibrationCallbacks(
				CalibrationStarted, CalibrationEnded, NULL, hCalibrationCBs);
		g_UserGenerator.GetPoseDetectionCap().RegisterToPoseCallbacks(PoseDetected,
				NULL, NULL, hPoseCBs);

		XnUserID aUsers[20];
		XnUInt16 nUsers = 20;
		g_UserGenerator.GetUsers(aUsers, nUsers);




		g_descriptionString.clear();
		g_descriptionString.push_back("Please Stand in front");
		g_descriptionString.push_back("of the Kinect as before,");
		g_descriptionString.push_back("new Player.");

		break;
	}
}

user Skeleton::getSkeleton() {

	user result;

	result.foot_left = btVector3(-5, 1, 0);
	result.knee_left = btVector3(-5, 5, 0);
	result.pelvis_left = btVector3(-3, 10, 0);
	result.foot_right = btVector3(5, 1, 0);
	result.knee_right = btVector3(5, 5, 0);
	result.pelvis_right = btVector3(3, 10, 0);
	result.chest = btVector3(0, 15, 0);
	result.shoulder_left = btVector3(-3, 20, 0);
	result.shoulder_right = btVector3(3, 20, 0);
	result.elbow_left = btVector3(-7, 20, 0);
	result.elbow_right = btVector3(7, 20, 0);
	result.hand_left = btVector3(-7, 25, 0);
	result.hand_right = btVector3(7, 25, 0);
	result.head = btVector3(0, 23, 0);

	if(g_bCalibrated){
		return m_current_user;
	}

	return result;
}

XnBool AssignPlayer(XnUserID user) {
	if (g_nPlayer != 0)
		return FALSE;

	XnPoint3D com;
	g_UserGenerator.GetCoM(user, com);
	if (com.Z == 0)
		return FALSE;

	printf("Matching for existing calibration\n");
	g_UserGenerator.GetSkeletonCap().LoadCalibrationData(user, 0);
	g_UserGenerator.GetSkeletonCap().StartTracking(user);
	g_nPlayer = user;
	return TRUE;

}


void XN_CALLBACK_TYPE NewUser(xn::UserGenerator& generator, XnUserID user, void* pCookie)
{
//	printf("nu user\n");
	if (!g_bCalibrated) // check on player0 is enough
	{
		g_descriptionString.clear();
		g_descriptionString.push_back("I see you. Please stand");
		g_descriptionString.push_back("in the same pose as");
		g_descriptionString.push_back("the dummy.");
		printf("Look for pose\n");
		g_UserGenerator.GetPoseDetectionCap().StartPoseDetection("Psi", user);
		return;
	}

//	AssignPlayer(user);
	// 	if (g_nPlayer == 0)
	// 	{
	// 		printf("Assigned user\n");
	// 		g_UserGenerator.GetSkeletonCap().LoadCalibrationData(user, 0);
	// 		g_UserGenerator.GetSkeletonCap().StartTracking(user);
	// 		g_nPlayer = user;
	// 	}
}

void FindPlayer() {
	if (g_nPlayer != 0) {
		return;
	}
	XnUserID aUsers[20];
	XnUInt16 nUsers = 20;
	g_UserGenerator.GetUsers(aUsers, nUsers);

	for (int i = 0; i < nUsers; ++i) {
		if (AssignPlayer(aUsers[i]))
			return;
	}
}

void LostPlayer() {
	g_nPlayer = 0;
	FindPlayer();
}

void XN_CALLBACK_TYPE LostUser(xn::UserGenerator& generator, XnUserID user, void* pCookie)
{
	g_descriptionString.clear();
	g_descriptionString.push_back("I lost you, please step in");
	g_descriptionString.push_back("front of the kinect.");
	printf("Lost user %d\n", user);
	if (g_nPlayer == user)
	{
		LostPlayer();

		if(g_bCalibrated){
			g_bCalibrated = false;
		}
	}
}

void XN_CALLBACK_TYPE PoseDetected(xn::PoseDetectionCapability& pose, const XnChar* strPose, XnUserID user, void* cxt)
{
	g_descriptionString.clear();
	g_descriptionString.push_back("I detected your pose, ");
	g_descriptionString.push_back("please hold that position.");
	g_descriptionString.push_back("Move around a little if");
	g_descriptionString.push_back("this takes too long.");
	printf("Found pose \"%s\" for user %d\n", strPose, user);
	g_UserGenerator.GetSkeletonCap().RequestCalibration(user, TRUE);
	g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(user);
}

void XN_CALLBACK_TYPE CalibrationStarted(xn::SkeletonCapability& skeleton, XnUserID user, void* cxt)
{
	printf("Calibration started\n");
}

#define GET_JOINT_CONFIDENCE(part)												\
		g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(user_id, part, joint);		\
		conf += joint.position.fConfidence;
//		printf("" #part ": %f\n",joint.position.fConfidence);


int g_getConfidence(XnUserID user_id) {
	XnSkeletonJointTransformation joint;

	int conf = 0;
	GET_JOINT_CONFIDENCE(XN_SKEL_HEAD);
	GET_JOINT_CONFIDENCE(XN_SKEL_LEFT_FOOT);
	GET_JOINT_CONFIDENCE(XN_SKEL_RIGHT_FOOT);
	GET_JOINT_CONFIDENCE(XN_SKEL_LEFT_SHOULDER);
	GET_JOINT_CONFIDENCE(XN_SKEL_RIGHT_SHOULDER);
	GET_JOINT_CONFIDENCE(XN_SKEL_LEFT_KNEE);
	GET_JOINT_CONFIDENCE(XN_SKEL_RIGHT_KNEE);
	GET_JOINT_CONFIDENCE(XN_SKEL_LEFT_HIP);
	GET_JOINT_CONFIDENCE(XN_SKEL_RIGHT_HIP);
	GET_JOINT_CONFIDENCE(XN_SKEL_LEFT_ELBOW);
	GET_JOINT_CONFIDENCE(XN_SKEL_RIGHT_ELBOW);
	GET_JOINT_CONFIDENCE(XN_SKEL_LEFT_HAND);
	GET_JOINT_CONFIDENCE(XN_SKEL_RIGHT_HAND);
	GET_JOINT_CONFIDENCE(XN_SKEL_TORSO);

	return conf;

}

void init_transform(XnUserID user) {
	int conf = 0;
	conf = g_getConfidence(user);


	while(conf < CONFIDENCE_TARGET_VALUE){
//		printf("confidence to low: %d\n", conf);
		g_Context.WaitAndUpdateAll();
		conf = g_getConfidence(user);
	}


	XnSkeletonJointTransformation joint;
	XnSkeletonJointTransformation joint2;
//	g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(user, XN_SKEL_HEAD, joint);
//	btVector3 v_head = btVector3(joint.position.position.X,joint.position.position.Y,joint.position.position.Z);
//	printf("head: %f x %f x %f\n", v_head.x(), v_head.y(), v_head.z());
//	g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(user, XN_SKEL_LEFT_HIP, joint);
//	g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(user, XN_SKEL_RIGHT_HIP, joint2);
//	btVector3 v_pubic = btVector3((joint.position.position.X - joint2.position.position.X)/2,
//			(joint.position.position.Y - joint2.position.position.Y)/2,
//			(joint.position.position.Z - joint2.position.position.Z)/2);

	g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(user, XN_SKEL_RIGHT_SHOULDER, joint);
	g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(user, XN_SKEL_LEFT_SHOULDER, joint2);
	btVector3 v_head = btVector3((joint.position.position.X - joint2.position.position.X)/2,
			(joint.position.position.Y - joint2.position.position.Y)/2,
			(joint.position.position.Z - joint2.position.position.Z)/2);

	g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(user, XN_SKEL_LEFT_HIP, joint);
	g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(user, XN_SKEL_RIGHT_HIP, joint2);
	btVector3 v_pubic = btVector3((joint.position.position.X - joint2.position.position.X)/2,
			(joint.position.position.Y - joint2.position.position.Y)/2,
			(joint.position.position.Z - joint2.position.position.Z)/2);



	// Calculate the rotation of the person.
	btVector3 given_up = v_pubic - v_head;
	btVector3 straight_up = btVector3(0.f,1.f,0.f);
	btVector3 rotation_axis = given_up.cross(straight_up);
	btScalar rotation_angle = -given_up.angle(straight_up);

	g_rotation_angle = rotation_angle;
	g_rotation_axis = rotation_axis;

	// Compute a Bounding Box around the skeleton.
//	printf("pubic-head: %f x %f x %f\n", given_up.x(), given_up.y(), given_up.z());
//	printf("axis: %f x %f x %f\n", rotation_axis.x(), rotation_axis.y(), rotation_axis.z());
//	printf("angle: %f\n", rotation_angle);
	g_scale_factor = 1;
	g_translation = btVector3(0,0,0);

	setPlayerJoints(user);

	// We now have the first valid values for the kinect data and can compute
	// a bounding box around the player, thus having scaling factors.
	btScalar height = g_user.head.y() - g_user.foot_left.y();

//	printf("height: %f\n", height);

	g_scale_factor = 1/height*WANTED_HEIGHT;

	if(height == 0 || g_scale_factor == 0){
		g_bCalibrated = false;
		printf("the calibration was not correct somewhere...\n");
		printf("Look for pose\n");
		g_UserGenerator.GetPoseDetectionCap().StartPoseDetection("Psi", user);
	}

	setPlayerJoints(user);
	g_translation = btVector3(g_user.head.x(),g_user.foot_left.y() + 1,g_user.head.z());
}

void XN_CALLBACK_TYPE CalibrationEnded(xn::SkeletonCapability& skeleton, XnUserID user, XnBool bSuccess, void* cxt)
{
	printf("Calibration done [%d] %ssuccessfully\n", user, bSuccess?"":"un");
	if (bSuccess)
	{
		if (!g_bCalibrated)
		{
//			g_UserGenerator.GetSkeletonCap().SaveCalibrationData(user, 0);
			g_nPlayer = user;
			g_UserGenerator.GetSkeletonCap().StartTracking(user);
			g_bCalibrated = TRUE;
		}

		XnUserID aUsers[10];
		XnUInt16 nUsers = 10;
		g_UserGenerator.GetUsers(aUsers, nUsers);
		for (int i = 0; i < nUsers; ++i)
			g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(aUsers[i]);


		// Get valuesd first and initialize the rotation
		g_Context.WaitAndUpdateAll();
		printf("start init transform!\n");
		init_transform(user);

	} else {
		g_descriptionString.clear();
		g_descriptionString.push_back("I messed up the cali-");
		g_descriptionString.push_back("bration. Please stand in");
		g_descriptionString.push_back("the same pose as the");
		g_descriptionString.push_back("dummy.");
		printf("Look for pose\n");
		g_UserGenerator.GetPoseDetectionCap().StartPoseDetection("Psi", user);
	}
}

#define SET_USER_JOINT(part, user_part )												\
		g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(user_id, part, joint);		\
		vector = joint.position.position; \
		user_joints.user_part = /*transformation(*/btVector3(vector.X*g_scale_factor, 			\
				vector.Y*g_scale_factor,vector.Z*g_scale_factor)/*)*/- g_translation;


void setPlayerJoints(XnUserID user_id) {
	// We want to get the player's joint in local player coordinates.
	// To do so, we assume, that the position and orientation of the kinect
	// sensor is not modified during tracking.
	btTransform transformation;
	transformation.setIdentity();
	transformation.setRotation(btQuaternion(g_rotation_axis,g_rotation_angle));

	user user_joints;
	XnSkeletonJointTransformation joint;
	XnVector3D vector;

	if(!g_UserGenerator.GetSkeletonCap().IsCalibrated(user_id)){

		g_nPlayer = 0;
		g_bCalibrated = false;

		return;
	}


	SET_USER_JOINT(XN_SKEL_HEAD, head);
	SET_USER_JOINT(XN_SKEL_LEFT_FOOT, foot_left);
	SET_USER_JOINT(XN_SKEL_RIGHT_FOOT, foot_right);
	SET_USER_JOINT(XN_SKEL_LEFT_SHOULDER, shoulder_left);
	SET_USER_JOINT(XN_SKEL_RIGHT_SHOULDER, shoulder_right);
	SET_USER_JOINT(XN_SKEL_LEFT_KNEE, knee_left);
	SET_USER_JOINT(XN_SKEL_RIGHT_KNEE, knee_right);
	SET_USER_JOINT(XN_SKEL_LEFT_HIP, pelvis_left);
	SET_USER_JOINT(XN_SKEL_RIGHT_HIP, pelvis_right);
	SET_USER_JOINT(XN_SKEL_LEFT_ELBOW, elbow_left);
	SET_USER_JOINT(XN_SKEL_RIGHT_ELBOW, elbow_right);
	SET_USER_JOINT(XN_SKEL_LEFT_HAND, hand_left);
	SET_USER_JOINT(XN_SKEL_RIGHT_HAND, hand_right);
	SET_USER_JOINT(XN_SKEL_TORSO, chest);

	g_user = user_joints;
}

void g_process() {
	// Main steps of the Skeleton loop!
//	g_Context.WaitAndUpdateAll();
	g_Context.WaitNoneUpdateAll();

	if (g_nPlayer != 0)
			{
		XnPoint3D com;
		g_UserGenerator.GetCoM(g_nPlayer, com);
		if (com.Z == 0) {
			g_nPlayer = 0;
			FindPlayer();
		}
		setPlayerJoints(g_nPlayer);

	}
}

void Skeleton::process() {
	// Main steps of the Skeleton loop!

	g_process();


	if(g_bCalibrated && !m_calibrated){
		Message msg;
		msg.messageType = Message::MSG_KINECT_CALIBRATED;
		this->sendMessage(msg);
		m_calibrated = true;

	} else if (!g_bCalibrated) {

		if(m_calibrated){
			Message msg;
			msg.messageType = Message::MSG_LOST_CALIBRATION;
			sendMessage(msg);

			msg.messageType = Message::MSG_STOP_KINECT;
			sendMessage(msg);
			m_calibrated = false;
		}
	}

	m_current_user = g_user;
//	getRandomFloat();
}

int Skeleton::getConfidence(){
	return g_getConfidence(g_nPlayer);
}

float Skeleton::getRandomFloat(){

	float random = 0;

	if(g_bCalibrated && false){
		xn::DepthMetaData depthMap;
		g_DepthGenerator.GetMetaData(depthMap);
		const XnDepthPixel* pixel = depthMap.Data();

		int sum = 0;
		int yres = depthMap.YRes();
		int xres = depthMap.XRes();

		for(int y = 0; y < yres; y ++){
			for(int x = 0; x < xres; x ++){
				sum += *pixel;
				pixel++;
			}
		}
		// Each pixel is a 16-bit value.
		random = (float)(sum % 100000)/100000;


		printf("%.10f\n", rand);

		last_rand = random;

	} else {
		random = (float)(rand()%100000)/100000;
//		printf("did not calibrate!\n");
	}


	return random;
}

XnStatus Skeleton::init_kinect() {
	printf("init kinect!\n");
	XnStatus rc = XN_STATUS_OK;

	rc = g_Context.InitFromXmlFile(SAMPLE_XML_PATH);
	CHECK_RC(rc, "InitFromXml");

//	try  {
////		rc = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
//	}catch (int a){
//		printf("depth_gen did not succeed\n");
//	}

	printf("after searching depth_gen\n");
	CHECK_RC(rc, "Find depth generator");
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);
	CHECK_RC(rc, "Find user generator");

	if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON)
			|| !g_UserGenerator.IsCapabilitySupported(
					XN_CAPABILITY_POSE_DETECTION)) {
		printf(
				"User generator doesn't support either skeleton or pose detection.\n");
		return XN_STATUS_ERROR;
	}

	g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

	rc = g_Context.StartGeneratingAll();
	CHECK_RC(rc, "StartGenerating");

	XnCallbackHandle hUserCBs, hCalibrationCBs, hPoseCBs;
	g_UserGenerator.RegisterUserCallbacks(NewUser, LostUser, NULL, hUserCBs);
	g_UserGenerator.GetSkeletonCap().RegisterCalibrationCallbacks(
			CalibrationStarted, CalibrationEnded, NULL, hCalibrationCBs);
	g_UserGenerator.GetPoseDetectionCap().RegisterToPoseCallbacks(PoseDetected,
			NULL, NULL, hPoseCBs);
	printf("init done!\n");
	g_bStartedKinect = true;

	return XN_STATUS_OK;
}

std::list<std::string> Skeleton::getDescriptionString()
{
	m_descriptionString.assign(g_descriptionString.begin(), g_descriptionString.end());
    return m_descriptionString;
}
