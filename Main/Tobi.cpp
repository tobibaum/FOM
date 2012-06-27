/*
 * Tobi.cpp
 *
 *  Created on: May 23, 2011
 *      Author: Tobi
 */

#include "Tobi.h"
#include "SignalStation.h"
#include "LoadWall.h"
#include "LooseCompoundShape.h"

using namespace std;

Tobi::Tobi() {
	// TODO Auto-generated constructor stub
	m_startFlight = false;
	setTrigger(Message::MSG_START_FLIGHT);
}

void Tobi::trigger(){

}

Tobi::~Tobi() {
	// TODO Auto-generated destructor stub
}


#define PRINT_JOINT(joint)      										\
		vec = current.joint;											\
		printf("" #joint ": %f x %f x %f\n",vec.x(),vec.y(),vec.z());

//#define DEBUG_RENDER
void Tobi::renderOverlay(){

#ifdef DEBUG_RENDER
//	glPushMatrix();
	float lightness = .4;
	glColor4f(lightness,lightness,lightness,0);
	glEnable(GL_LIGHTING);
	float frameDepth = 0;

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(.1,.1,.1);
	glMatrixMode(GL_MODELVIEW);

	static const GLfloat	planex[]={1,0,0,0};
	static const GLfloat	planey[]={0,1,0,0};
	static const GLfloat	planez[]={0,0,1,0};
	glTexGenfv(GL_S,GL_OBJECT_PLANE,planex);
	glTexGenfv(GL_T,GL_OBJECT_PLANE,planey);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);

//	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("redStone.jpg"));

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_CULL_FACE);

	glBegin(GL_TRIANGLE_STRIP);

//		btVector3 diff1 = tri_it->p2 - tri_it->p1;
//		btVector3 diff2 = tri_it->p3 - tri_it->p1;
//		btVector3 normal = diff1.cross(diff2);
//
//		normal.normalize();
		list<btVector3>::iterator vec_it;
		vec_it = m_points.begin();

		for(;
			vec_it != m_points.end(); vec_it++){
			glTexCoord2f(0,vec_it->getX());
			glVertex3f(vec_it->getX(),vec_it->getY(),vec_it->getZ());
			glTexCoord2f(frameDepth,vec_it->getX());
			glVertex3f(vec_it->getX(),vec_it->getY(),vec_it->getZ() + frameDepth);
		}
		vec_it = m_points.begin();
		glTexCoord2f(0,vec_it->getX());
		glVertex3f(vec_it->getX(),vec_it->getY(),vec_it->getZ());
		glTexCoord2f(frameDepth,vec_it->getX());
		glVertex3f(vec_it->getX(),vec_it->getY(),vec_it->getZ() + frameDepth);

	glEnd();

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);

	glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("wall.jpg"));

	glColor4f(1,1,1,0.5);

	glBegin(GL_TRIANGLES);
		for(list<triangle>::iterator tri_it = m_triangles.begin();tri_it != m_triangles.end();tri_it++){

			btVector3 normal = btVector3(0,0,-1);
//
			glNormal3f(normal.getX(),normal.getY(),normal.getZ());
			glVertex3f(tri_it->p1[0],tri_it->p1[1],tri_it->p1[2] + frameDepth);
			glNormal3f(normal.getX(),normal.getY(),normal.getZ());
			glVertex3f(tri_it->p2[0],tri_it->p2[1],tri_it->p2[2] + frameDepth);
			glNormal3f(normal.getX(),normal.getY(),normal.getZ());
			glVertex3f(tri_it->p3[0],tri_it->p3[1],tri_it->p3[2] + frameDepth);
		}
	glEnd();

	glColor4f(1,1,1,0);





	glColor4f(0,1,0,0);

	glBegin(GL_LINE);
	for(list<triangle>::iterator tri_it = m_triangles.begin();tri_it != m_triangles.end();tri_it++){
//		glVertex3f(tri_it->p1[0],tri_it->p1[1],tri_it->p1[2]);
//		glVertex3f(tri_it->p2[0],tri_it->p2[1],tri_it->p2[2]);
//		glVertex3f(tri_it->p2[0],tri_it->p2[1],tri_it->p2[2]);
//		glVertex3f(tri_it->p3[0],tri_it->p3[1],tri_it->p3[2]);
//		glVertex3f(tri_it->p3[0],tri_it->p3[1],tri_it->p3[2]);
//		glVertex3f(tri_it->p1[0],tri_it->p1[1],tri_it->p1[2]);
	}
	glEnd();

	glColor4f(1,0,0,0);

	glBegin(GL_LINE);
//		list<btVector3>::iterator vec_it;
		vec_it = m_points.begin();
		glVertex3f(vec_it->getX(),vec_it->getY(),vec_it->getZ());
		vec_it++;
		for(;
			vec_it != m_points.end(); vec_it++){
			glVertex3f(vec_it->getX(),vec_it->getY(),vec_it->getZ());
			glVertex3f(vec_it->getX(),vec_it->getY(),vec_it->getZ());
		}
	glEnd();


//	glPopMatrix();

#endif
}

bool doit = false;

void Tobi::step(){

	m_player->updatePlayer();
	m_tracker->process();

	if(m_gameStarted && !m_pause){
		if(m_currentMode == ArcadeConfiguration::MODE_ARCADE){
			m_canon->process();
		}
		if(m_currentMode == ArcadeConfiguration::MODE_SPARTA){
			m_spartaCannon->process();
		}
	}

//	user current = m_skel->getSkeleton();
//	btVector3 vec;
//	PRINT_JOINT(foot_left);
//	PRINT_JOINT(foot_right);
//	PRINT_JOINT(head);
//	PRINT_JOINT(chest);


//	m_canon->process();

//	m_player->updatePlayer();
//	m_tracker->process();
//	m_menu->process();
//	m_canon->process();
//	if(m_gameStarted && !m_pause){
////		m_canon->process();
//		m_spartaCannon->process();
//	}



	if(doit){

#ifdef DEBUG_RENDER
		LoadWall* loadWall = LoadWall::getInstance();
		m_points = loadWall->getPoints("easy3.png",40);
//		m_triangles = loadWall->getTriangles("spongebob.png",50);
		m_triangles = loadWall->getInnerTriangles("easy3.png",40);
#endif

		btTransform wallTrans;
		wallTrans.setIdentity();
		wallTrans.setOrigin(btVector3(0,0,-100));
		LooseCompoundShape* wallbody = m_coloredWorld->addWallObject(wallTrans,"bend.png",5,40);
		wallbody->setVelocity(btVector3(0,0,60));
//		delete wallbody;
//		((LooseRigidBody*)wallbody)->setWallVelocity(btVector3(0,5,40));
//		m_coloredWorld->addTexture(wallbody,"beton.bmp");
		doit=false;
	}


//	m_menu->process();
//
//	if(!m_ended){
//		m_player->updatePlayer();
//	}
//
//	if(m_gameStarted && !m_pause){
//		m_canon->process();
//	}


// The initial camera flight!

	btVector3 pos = m_camera->getPosition();

	if(pos.distance(btVector3(100,30,20)) < 4){
		m_camera->flyCameraTo();
		m_camera->setTarget(btVector3(0,17,0));
		m_camera->setPosition(btVector3(0,30,30));
	}

	if(m_startFlight){
		m_startFlight = false;

		m_camera->flyCameraTo();

		m_camera->setPosition(btVector3(100,30,20));
	}

	if(pos.distance(btVector3(0,30,30)) < 4)
	{
		if(m_showFOM){
			Message msg;
			msg.messageType = Message::MSG_FLIGHT_FINISHED;
			sendMessage(msg);
		}
	}
}

void Tobi::initWorld(){


//	createStadion();

//
//	m_canon->setFrequency(.8);
//	m_canon->setSpeed(250);
//	m_canon->set_target(btVector3(8,23,0));
//
//

	m_camera->setPosition(btVector3(0,10,-110));
	m_camera->setTarget(btVector3(0,0,0));

//	m_camera->setColorFilter(btVector3(0,1,0));

//	std::string load = "SP_Easy.xml";
//
//	SpartaConfiguration* config = m_configLoader.readSpartaFile(load);
//	m_spartaCannon->setConfiguration(config);
//	SpartaLevel* level = config->getLevel(1);
//	printf("lifes: %d\n",level->getLiveThresh());
//
//	LevelHandler::getInstance()->setSpartaConfiguration(config);
//	LevelHandler::getInstance()->setCurrentLevel(1);
//	m_spartaCannon->start();


//	m_castle = new Castle(0,-28);

//	m_camera->setPosition(btVector3(0,30,40));

//	btCollisionShape* shape = m_objloader->loadObject("bunny.obj", 30);


}

