/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


//define the dimensions of the stadium
#define STADION_X 60
#define STADION_Y 5
#define STADION_Z 200


//maximum number of objects (and allow user to shoot additional boxes)
#define MAX_PROXIES (ARRAY_SIZE_X*ARRAY_SIZE_Y*ARRAY_SIZE_Z + 1024)

#include "Field.h"
#include "GlutStuff.h"

///btBulletDynamicsCommon.h is the main Bullet include file, contains most common include files.
#include "btBulletDynamicsCommon.h"

#include <stdio.h> //printf debugging
#include <stdlib.h>
#include <string>
#include <sstream>

using namespace std;

void Field::clientMoveAndDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	//simple dynamics world doesn't handle fixed-time-stepping
	float ms = getDeltaTimeMicroseconds();
//	printf("%f\n", ms);
//	ms= 100000000;

	///step the simulation
	if (m_coloredWorld)
	{
		m_coloredWorld->stepSimulation(ms / 1000000.f);

		//optional but useful: debug drawing

		step();

		m_castlePointCount->process();
		m_skel->process();
		m_collDect->process();
		SignalStation::getInstance()->process();
		m_camera->process(ms/1000);
		m_cleaner->process();
		m_levelHandler->process();
		m_menu->process(ms/1000);

//		m_colorer->process();

//		m_dynamicsWorld->debugDrawWorld();
	}

	renderSkyBox();

	glPushMatrix();
	renderme(0);
	glPopMatrix();

	drawSpot();

	glPushMatrix();
	renderme(1);
	glPopMatrix();



	glBegin(GL_LINE);
		glColor3f(1,0,0);
		glVertex3f(10,10,10);
		glVertex3f(0,0,0);
	glEnd();



	glPushMatrix();
	setOrthographicProjection();

	if(m_showExplanation){
		glColor4f(0,0.5,0.2,0.5);
		drawRectangleOnScreen(1000,5,1350,150);

		glColor4f(0.0,0.6,0.3,0.75);
		drawRectangleOnScreen(1005,10,1355,155);

		glColor4f(1,1,1,0);
		list<string> explanationList = m_levelHandler->getDescriptionString();
		int startY=40;
		list<string>::iterator it;
		for(it = explanationList.begin(); it != explanationList.end(); it++){
			renderString(*it, 1010, startY);
			startY+= 30;
		}
	}

	if(m_showCalibrateHelpMessage){
		glColor4f(0,0.5,0.2,0.5);
		drawRectangleOnScreen(1000,5,1350,150);

		glColor4f(0.0,0.6,0.3,0.75);
		drawRectangleOnScreen(1005,10,1355,155);

		glColor4f(1,1,1,0);
		list<string> explanationList = m_skel->getDescriptionString();
		int startY=20;
		list<string>::iterator it;
		for(it = explanationList.begin(); it != explanationList.end(); it++){
			renderString(*it, 1010, startY);
			startY+= 30;
		}
	}

	if(m_gameStarted){
		// print point-string on screen

		// POINTS
		glColor4f(0,0,0,0.5);
		drawRectangleOnScreen(15,0,180,30);

		glColor4f(0.3,0.3,0.3,0.75);
		drawRectangleOnScreen(20,5,185,35);

		//BALL
		glColor4f(0,0,0,0.5);
		drawRectangleOnScreen(15,40,180,70);

		glColor4f(0.3,0.3,0.3,0.75);
		drawRectangleOnScreen(20,45,185,75);

		//TIME
		glColor4f(0,0,0,0.5);
		drawRectangleOnScreen(15,80,180,110);

		glColor4f(0.3,0.3,0.3,0.75);
		drawRectangleOnScreen(20,85,185,115);

		//LIVES
		glColor4f(0,0,0,0.5);
		drawRectangleOnScreen(15,120,180,150);

		glColor4f(0.3,0.3,0.3,0.75);
		drawRectangleOnScreen(20,125,185,155);

		//OVERALL POINTS
		glColor4f(0,0,0,0.5);
		drawRectangleOnScreen(15,170,210,220);

		glColor4f(0.3,0.3,0.3,0.75);
		drawRectangleOnScreen(20,175,215,225);

		if(m_ended){
			glColor4f(0.1,0.1,0.1,0.5);
			drawRectangleOnScreen(0,0,1358,668);
		}

		glColor4f(1,1,1,0);


		//print points
		char printChar[128];
		if(m_levelHandler->getPoints() != -1){
			sprintf(printChar, "points: %d", m_levelHandler->getPoints());
		} else{
			sprintf(printChar, "points: inf");
		}
		string printString = printChar;
		renderString(printString, 20, 20);

		//print balls
		char printChar2[128];
		if(m_levelHandler->getBallsRemaining() != -1){
			sprintf(printChar2, "balls: %d", m_levelHandler->getBallsRemaining());
		} else {
			sprintf(printChar2, "balls: inf");
		}

		string printString2 = printChar2;
		renderString(printString2, 20, 60);

		//print time
		char printChar3[128];
		if(m_levelHandler->getTimeRemaining() != -1){
			sprintf(printChar3, "time: %d", m_levelHandler->getTimeRemaining());
		} else {
			sprintf(printChar3, "time: inf");
		}
		string printString3 = printChar3;
		renderString(printString3, 20, 100);


		//print lives
		char printChar4[128];
		if(m_levelHandler->getLivesRemaining() != -1 ){
			sprintf(printChar4, "lives: %d", m_levelHandler->getLivesRemaining());
		} else {
			sprintf(printChar4, "lives: inf");
		}
		string printString4 = printChar4;
		renderString(printString4, 20, 140);

		//print overall points
		char printChar5[128];
		char printChar6[128];
		sprintf(printChar5, "Overall points:");
		sprintf(printChar6, "     %d", (m_pointing->getOverallPoints() + m_pointing->getPoints()));
		string printString5 = printChar5;
		string printString6 = printChar6;
		renderString(printString5, 20, 180);
		renderString(printString6, 20, 210);
	}

	if(m_highscoreCalled){
			// PRINT HIGHSCORERS


			glColor4f(0,0,0,0.5);
			drawRectangleOnScreen(500,10,900,500);

			glColor4f(0.3,0.3,0.3,0.75);
			drawRectangleOnScreen(505,15,905,505);

			glColor4f(1,1,1,0);

			list<PlayerPoints>::iterator it;

			int xStart = 510;
			int yStart = 30;
			int rank = 1;

			string diff = "--- " + m_highscoreDiffState + " ---";
			renderString(diff, xStart, yStart);
			yStart+= 40;

			for(it = m_tempHighscoreList.begin(); it != m_tempHighscoreList.end(); it++){

				ostringstream ssRank;
				ssRank << rank;
				string output = ssRank.str() + "." + "  " + it->name + " - ";
				ostringstream ss;
				ss << it->points;
				output = output + ss.str();
				renderString(output, xStart, yStart);
				yStart += 30;
				rank++;
			}

	}

	if(m_showStatistics){
					// PRINT STATISTICS


					glColor4f(0,0,0,0.5);
					drawRectangleOnScreen(500,10,900,500);

					glColor4f(0.3,0.3,0.3,0.75);
					drawRectangleOnScreen(505,15,905,505);

					glColor4f(1,1,1,0);

					int xStart = 510;
					int yStart = 30;
					int rank = 1;

					for(int i=0; i<6; i++){
						if(i==0){
							string objectString = "Bad Object hits : ";
							ostringstream ss;
							ss << m_levelHandler->getBadHits();
							string hits = ss.str();
							objectString = objectString + hits + "\n";
							renderString(objectString,xStart,yStart);
						}
						if(i==1){
							string objectString = "Good Object hits : ";
							ostringstream ss;
							ss << m_levelHandler->getGoodHits();
							string hits = ss.str();
							objectString = objectString + hits + "\n";
							renderString(objectString,xStart,yStart);
						}
						if(i==2){
							string objectString = "Reflect Object hits : ";
							ostringstream ss;
							ss << m_levelHandler->getReflectHits();
							string hits = ss.str();
							objectString = objectString + hits + "\n";
							renderString(objectString,xStart,yStart);
							yStart += 20;
						}
						if(i==3){
							string objectString = "Ball Bonus : ";
							ostringstream ss;
							ss << m_levelHandler->getBonusBalls();
							string bonus = ss.str();
							objectString = objectString + bonus + "\n";
							renderString(objectString,xStart,yStart);
						}
						if(i==4){
							string objectString = "Time Bonus : ";
							ostringstream ss;
							ss << m_levelHandler->getBonusTime();
							string bonus = ss.str();
							objectString = objectString + bonus + "\n";
							renderString(objectString,xStart,yStart);
						}
						if(i==5){
							string objectString = "Life Bonus : ";
							ostringstream ss;
							ss << m_levelHandler->getBonusLifes();
							string bonus = ss.str();
							objectString = objectString + bonus + "\n";
							renderString(objectString,xStart,yStart);
						}
						yStart += 30;
					}




			}


		// --------- FOM START TEXTURE
		if(m_showFOM){
			glMatrixMode(GL_TEXTURE);

			glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);
			glScalef(40,40,40);

			// fade the text according to distance to player
			float value ;
			if(m_fShowFOMTime == 0){
				value = 1;
			} else {
				value = 1 - m_fShowFOMTime/3250625;
				m_fShowFOMTime+=ms;
			}


			if(m_startFlight){
				m_fShowFOMTime+=ms;
			}

			glColor4f(1,1,1,value);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//		glBlendFunc(GL_DST_COLOR,GL_ZERO);
			glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("FOM.png"));

	//		glColor4f(1,0,0,0);

			// 1358 x 668
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);
				glVertex2f(0, 0);
				glTexCoord2f(0,1);
				glVertex2f(0, 668);
				glTexCoord2f(1,1);
				glVertex2f(1358, 668);
				glTexCoord2f(1,0);
				glVertex2f(1358, 0);
			glEnd();

			glPopMatrix();
		}

		if(m_instructionMenu){
			glMatrixMode(GL_TEXTURE);

			glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);
			glScalef(40,40,40);

			// fade the text according to distance to player

			glColor4f(1,1,1,1);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//		glBlendFunc(GL_DST_COLOR,GL_ZERO);
			glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("FOM_Instructions.png"));

	//		glColor4f(1,0,0,0);

			// 1358 x 668
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);
				glVertex2f(0, 0);
				glTexCoord2f(0,1);
				glVertex2f(0, 668);
				glTexCoord2f(1,1);
				glVertex2f(1358, 668);
				glTexCoord2f(1,0);
				glVertex2f(1358, 0);
			glEnd();

			glPopMatrix();

		}

		resetPerspectiveProjection();



	glPopMatrix();

	renderOverlay();

	glFlush();

	swapBuffers();

	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
}

void Field::drawSpot(){

	if(m_showSpotOnGround){
		glClear(GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glDisable(GL_CULL_FACE);
		glDisable(GL_TEXTURE_2D);
		// We want the user to stand right in the front of our menu. We force that with a green spot
		// on the ground.
		glRotatef(-90,1,0,0);
		// setup code
		static GLUquadricObj *q;
		q = gluNewQuadric();
		gluQuadricNormals (q,GLU_TRUE);
		gluQuadricTexture (q,GLU_TRUE);
		gluQuadricDrawStyle(q, GLU_FILL);

		glColor4f(0, 1, 0, .5);

		// the gluDisk based shapes
		gluDisk (q, 0.0, 7, 15, 15);  // Solid Circle
		glPopMatrix();
		glEnable(GL_CULL_FACE);
	}

}

Field::Field(){
	m_gameStarted = false;
	m_pause = false;
	m_ended = false;
	m_highscoreCalled = false;
	m_showStatistics = false;
	m_showExplanation = false;
	m_showCalibrateHelpMessage = false;
	m_showFOM = true;
	m_instructionMenu = false;
	m_showSpotOnGround = false;

//	string load = "Highscores.xml";
//	m_tempHighscoreList = m_highscoreHandler.readFile(load);
}

void Field::renderString(string output, int x, int y){
	glRasterPos2i(x,y);
	char* c;
	void* font = GLUT_BITMAP_HELVETICA_18;
	int space = 5;
	int startY = y;
	int startX = x;
	int xPos = startX;
	char* printString = (char*)output.c_str();

	for(c = printString ; *c != '\0' ; c++){
		glRasterPos2f(xPos, startY);
		glutBitmapCharacter(font,*c);
		xPos = xPos + glutBitmapWidth(font,*c) + space;
	}
}

void Field::drawRectangleOnScreen(int x1, int y1, int x2, int y2){

	glBegin(GL_QUADS);
	glVertex2d(x1, y1);
	glVertex2d(x1, y2);
	glVertex2d(x2, y2);
	glVertex2d(x2, y1);
	glEnd();
}


void Field::renderSkyBox(){
	m_camPos = getCameraPosition();
//

	glNormal3f(-1,-1,-1);
	glColor4f(1,1,1,0);

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glPushMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);

	glEnable(GL_TEXTURE_2D);

	// front
	glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("skybox/front.bmp"));
	renderSkyPlane(-1,-1,-1,
			1,-1,-1,
			1,1,-1,
			-1,1,-1);

	// back
	glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("skybox/back.bmp"));
	renderSkyPlane(
			1,-1,1,
			-1,-1,1,
			-1,1,1,
			1,1,1);

	//left
	glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("skybox/left.bmp"));
	renderSkyPlane(-1,-1,1,
			-1,-1,-1,
			-1,1,-1,
			-1,1,1);


	//right
	glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("skybox/right.bmp"));
	renderSkyPlane(1,-1,-1,
			1,-1,1,
			1,1,1,
			1,1,-1);

	//bottom
	glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("skybox/bottom.bmp"));
	renderSkyPlane(
			-1,-1,1,
			1,-1,1,
			1,-1,-1,
			-1,-1,-1
			);

	//top
	glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("skybox/top.bmp"));
	renderSkyPlane(
			-1,1,-1,
			1,1,-1,
			1,1,1,
			-1,1,1
			);
	glPopMatrix();
}

void Field::renderSkyPlane(int m11, int m12,int m13,
		int m21,int m22,int m23,
		int m31,int m32,int m33,
		int m41,int m42,int m43){

	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex3f(m_camPos[0] + 5*m11, m_camPos[1] + 5*m12, m_camPos[2]  + 5*m13);
	glTexCoord2f(1,1);
	glVertex3f(m_camPos[0] + 5*m21, m_camPos[1] + 5*m22, m_camPos[2]  + 5*m23);
	glTexCoord2f(1,0);
	glVertex3f(m_camPos[0] + 5*m31, m_camPos[1] + 5*m32, m_camPos[2]  + 5*m33);
	glTexCoord2f(0,0);
	glVertex3f(m_camPos[0] + 5*m41, m_camPos[1] + 5*m42, m_camPos[2]  + 5*m43);
	glEnd();

}

void Field::displayCallback(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	
	clientMoveAndDisplay();
//	renderme();

	//optional but useful: debug drawing to detect problems
	if (m_coloredWorld)
		//m_dynamicsWorld->debugDrawWorld();


	glFlush();
//	swapBuffers();
}

bool	Field::initPhysics()
{
	setTexturing(true);
	setShadows(true);

	setCameraDistance(btScalar(13.));

	///collision configuration contains default setup for memory, collision setup
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	//m_collisionConfiguration->setConvexConvexMultipointIterations();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

	m_broadphase = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	m_solver = sol;

//	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	m_coloredWorld = new ColoredWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	
	m_coloredWorld->setGravity(btVector3(0,-10,0));

//	btTransform lightTrans;
//	lightTrans.setOrigin(btVector3(0,150,0));
//	btRigidBody* light = m_coloredWorld->addFlyingObject(1,lightTrans,new btSphereShape(5),FlyingObject::FOM_GOOD);
//	light->setGravity(btVector3(0,0,0));

	learnDynamicsWorld();

	m_camera = new Camera(this);

	m_pointing = PointingSystem::getInstance();

	m_cleaner = Cleaner::getInstance();

	setTrigger(Message::MSG_START_GAME);
	setTrigger(Message::MSG_PAUSE_GAME);
	setTrigger(Message::MSG_EXIT_CALL);
	setTrigger(Message::MSG_HIGHSCORE_CALL);
	setTrigger(Message::MSG_HIGHSCORE_DIFF_SELECTED);
	setTrigger(Message::MSG_END_GAME);
	setTrigger(Message::MSG_END_GAME_WITHOUT_HS);
	setTrigger(Message::MSG_WON);
	setTrigger(Message::MSG_NAME_ENTERED);
	setTrigger(Message::MSG_NEXT_LEVEL);
	setTrigger(Message::MSG_TOGGLE_EXPLANATION);
	setTrigger(Message::MSG_GAME_GO);
	setTrigger(Message::MSG_KINECT_CALIBRATED);
	setTrigger(Message::MSG_FLIGHT_FINISHED);
	setTrigger(Message::MSG_RETURN_GAME);
	setTrigger(Message::MSG_LOST_CALIBRATION);
	setTrigger(Message::MSG_INSTRUCTIONS);

	m_menu = new MenuList(m_coloredWorld);
	m_menu->setPlayerPosition(new btVector3(5,0,0));

//	m_speech = new SpeechSynch();

	m_collDect = CollisionDetector::getInstance();

	m_player = new Player(m_coloredWorld);
	m_player->createBodyShapes();

	m_spartaCannon = new SpartaCannon();

	m_tracker = HandTracker::getInstance();

	m_canon = new Cannon(m_coloredWorld);
	m_canon->set_position(btVector3(4,0,-100));

	m_sounds = new SoundManager();

	m_castlePointCount = CastlePointCounter::getInstance();

	m_colorer = Colorer::getInstance();

	createStadion();

	m_levelHandler = LevelHandler::getInstance();

	m_fShowFOMTime = 0;

	srand(time(NULL));

	m_skel = Skeleton::getInstance();

	if(m_skel->init_kinect()!= XN_STATUS_OK){
		// Kinect was not succesful. We should break now
//		printf("break!!\n");
		return false;
	}

	return true;

}


void Field::trigger(Message msg){

	string load;
	string hsFile;
	Message sendmsg;

	switch(msg.messageType){
	case Message::MSG_START_GAME:
		m_camera->flyCameraTo();
		m_camera->setPosition(btVector3(0,21.446262,12.216004));
		startGame((ArcadeConfiguration::difficulties)msg.additionalInfo, (ArcadeConfiguration::gameMode)msg.gameMode);
		m_pause = false;
		m_showSpotOnGround = false;
		break;
	case Message::MSG_PAUSE_GAME:
		if(!m_pause && m_gameStarted){
			m_menu->invokeMenu(MenuList::PAUSE_MENU);
			m_coloredWorld->freeze();
			m_pause = true;
			m_showSpotOnGround = true;
		}
		else{
//			m_menu->invokeMenu(m_menu->getActualMenu);
		}
		break;
	case Message::MSG_RETURN_GAME:
		if(m_pause && m_gameStarted){
			m_pause = false;
			m_coloredWorld->unfreeze();
			m_showSpotOnGround = false;
		}
		break;
	case Message::MSG_KINECT_CALIBRATED:
		m_showCalibrateHelpMessage = false;
		if(m_gameStarted == false){
			m_menu->createMainMenu();
		}
		break;
	case Message::MSG_FLIGHT_FINISHED:
		m_showFOM = false;
		m_showCalibrateHelpMessage = true;
		m_showSpotOnGround = true;
		break;
	case Message::MSG_START_FLIGHT:
		if(!m_bFlightdone){
			m_startFlight = true;
		}
		break;
	case Message::MSG_GAME_GO:
		m_gameStarted = true;
		m_pause = false;
		m_showSpotOnGround = false;
		break;

	case Message::MSG_LOST_CALIBRATION:
//		delete m_menu;
//		m_menu = new MenuList(m_coloredWorld);
		m_menu->closeMenu();

		m_showCalibrateHelpMessage = true;
		m_showSpotOnGround = true;
		break;
	case Message::MSG_WON:
		if(!m_pause){
			if(m_levelHandler->getMaxLevels() == m_levelHandler->getCurrentLevel()){
				m_menu->createWonMenu();
				m_pause = true;
				m_showExplanation = false;
				m_gameStarted = false;
			}
			else{
				// not last level
				m_menu->invokeMenu(MenuList::STATISTICS_MENU);
				m_showStatistics = true;
				m_pause = true;
			}
			m_showSpotOnGround = true;
		}
		break;
	case Message::MSG_NEXT_LEVEL:
		m_levelHandler->setCurrentLevel(m_levelHandler->getCurrentLevel() + 1);
		m_levelHandler->resetStatistics();
//		m_pause = false;
		m_showStatistics = false;
		m_canon->updateLevelData(m_levelHandler->getCurrentLevel());
		m_menu->closeMenu();

		m_menu->invokeMenu(MenuList::GO_MENU);
		m_showSpotOnGround = true;

		break;
	case Message::MSG_END_GAME:
//		m_menu->createEndMenu();

		m_showStatistics = false;
		m_gameStarted = false;
		m_showExplanation = false;

		m_camera->flyCameraTo();
		m_camera->setTarget(btVector3(0,17,0));
		m_camera->setPosition(btVector3(0,30,30));

		m_menu->clearMenuPoints();
		switch(m_currentDifficulty){
		case ArcadeConfiguration::DIFF_EASY : hsFile = "Highscores_EASY.xml";
											  break;
		case ArcadeConfiguration::DIFF_MEDIUM : hsFile = "Highscores_MEDIUM.xml";
											  break;
		case ArcadeConfiguration::DIFF_HARD : hsFile = "Highscores_HARD.xml";
											  break;
		case ArcadeConfiguration::DIFF_MADNESS : hsFile = "Highscores_MADNESS.xml";
											  break;
		}
		if(m_currentMode == ArcadeConfiguration::MODE_SPARTA){
			hsFile = "SP_" + hsFile;
		}
		hsFile = "../Highscores/" + hsFile;
		if(!(m_currentDifficulty == ArcadeConfiguration::DIFF_TRAINING) && m_highscoreHandler.reachedHighscore(hsFile, m_pointing->getOverallPoints())){
			m_menu->invokeMenu(MenuList::HIGHSCORE_MENU);
		}
		else{
			m_menu->invokeMenu(MenuList::MAIN_MENU);
		}

		for(list<Castle*>::iterator cit = castleList.begin(); cit != castleList.end(); cit++){
			delete *cit;
		}
//		m_camera->setColorFilter(btVector3(0.2,0.2,0.2));
//		delete m_player;
//		delete m_canon;
		break;
	case Message::MSG_END_GAME_WITHOUT_HS:
//		m_menu->createEndMenu();

		m_showStatistics = false;
		m_gameStarted = false;
		m_showExplanation = false;

		m_camera->flyCameraTo();
		m_camera->setTarget(btVector3(0,17,0));
		m_camera->setPosition(btVector3(0,30,30));

		m_menu->clearMenuPoints();

		for(list<Castle*>::iterator cit = castleList.begin(); cit != castleList.end(); cit++){
			delete *cit;
		}

		m_menu->invokeMenu(MenuList::MAIN_MENU);
		m_showSpotOnGround = true;

//		m_camera->setColorFilter(btVector3(0.2,0.2,0.2));
//		delete m_player;
//		delete m_canon;
		break;
	case Message::MSG_EXIT_CALL:
//		delete m_player;

		m_camera->flyCameraTo();
		m_camera->setPosition(btVector3(0,17,4));
		m_camera->setTarget(btVector3(0,17,-3));

		m_menu->createEndMenu();
		m_showSpotOnGround = true;
		m_ended = true;

		break;
	case Message::MSG_TOGGLE_EXPLANATION:
		if(!m_showExplanation){
			m_showExplanation = true;
			m_showSpotOnGround = true;
		}else{m_showExplanation = false;}
		break;

	case Message::MSG_HIGHSCORE_CALL:
	//		delete m_player;

			m_highscoreTypeRequested = msg.additionalInfo;
			if(!m_highscoreCalled){
				m_highscoreCalled = true;

			}
			else{m_highscoreCalled = false;}
			m_showSpotOnGround = true;
		break;
	case Message::MSG_HIGHSCORE_DIFF_SELECTED:
		m_highscoreDiffState = msg.stringInfo;
		m_highscoreCalled = true;
		if(m_highscoreDiffState.compare("BACK") == 0){
			m_highscoreCalled = false;
		}

		printf("%s\n",msg.stringInfo.c_str());
		if(m_highscoreDiffState.compare("EASY") == 0){
//			printf("I LOAD EASY HS\n");
			load = "Highscores_EASY.xml";
		}
		else if(m_highscoreDiffState.compare("MEDIUM") == 0){
			load = "Highscores_MEDIUM.xml";
		}
		else if(m_highscoreDiffState.compare("HARD") == 0){
			load = "Highscores_HARD.xml";
		}
		else if(m_highscoreDiffState.compare("MADNESS") == 0){
			load = "Highscores_MADNESS.xml";
		}

		if(m_highscoreTypeRequested == MenuList::HS_CALL_SPARTA){
			load = "SP_" + load;
		}
		load = "../Highscores/" + load;
		if(m_highscoreCalled){
			m_tempHighscoreList = m_highscoreHandler.readFile(load);
		}
		m_showSpotOnGround = true;

		break;
	case Message::MSG_NAME_ENTERED:
//		delete m_player;

		switch(m_currentDifficulty){
		case ArcadeConfiguration::DIFF_EASY : hsFile = "Highscores_EASY.xml";
											  m_highscoreDiffState = "EASY";
											  break;
		case ArcadeConfiguration::DIFF_MEDIUM : hsFile = "Highscores_MEDIUM.xml";
												m_highscoreDiffState = "MEDIUM";
											  break;
		case ArcadeConfiguration::DIFF_HARD : hsFile = "Highscores_HARD.xml";
											  m_highscoreDiffState = "HARD";
											  break;
		case ArcadeConfiguration::DIFF_MADNESS : hsFile = "Highscores_MADNESS.xml";
												 m_highscoreDiffState = "MADNESS";
											  break;
		}
		if(m_currentMode == ArcadeConfiguration::MODE_SPARTA){
			hsFile = "SP_" + hsFile;
			m_highscoreTypeRequested = MenuList::HS_CALL_SPARTA;
		}
		hsFile = "../Highscores/" + hsFile;
		m_highscoreHandler.addToHighscoreList(hsFile, msg.stringInfo, m_pointing->getOverallPoints());
//		m_menu->clearMenuPoints();
		sendmsg.messageType = Message::MSG_CURRENT_DIFFICULTY;
		sendmsg.additionalInfo = m_currentDifficulty;
		sendMessage(sendmsg);
		m_menu->invokeMenu(MenuList::HIGHSCORE_WATCH_MENU);

		m_showSpotOnGround = true;
		m_highscoreCalled = true;

		m_tempHighscoreList = m_highscoreHandler.readFile(hsFile);

		break;
		case Message::MSG_INSTRUCTIONS:
			m_instructionMenu = !m_instructionMenu;
			m_showSpotOnGround = true;
			break;
	}
}

void Field::createStadion(){
	// Create the Stadion
	btVector3 stadion_dimensions = btVector3(STADION_X,STADION_Y,STADION_Z);

	btCompoundShape* stadion = new btCompoundShape();//static scenario

	btCollisionShape* ground = new btBoxShape(btVector3(stadion_dimensions[0],1,stadion_dimensions[2]));
	btCollisionShape* left = new btBoxShape(btVector3(1,stadion_dimensions[1],stadion_dimensions[2]));
	btCollisionShape* right = new btBoxShape(btVector3(1,stadion_dimensions[1],stadion_dimensions[2]));
	btCollisionShape* front = new btBoxShape(btVector3(stadion_dimensions[0],stadion_dimensions[1],1));
	btCollisionShape* back = new btBoxShape(btVector3(stadion_dimensions[0],stadion_dimensions[1],1));

	btTransform	stadion_transform;
	stadion_transform.setIdentity();

	stadion_transform.setOrigin(btVector3(0,-1,0));
	stadion->addChildShape(stadion_transform,ground);
	stadion_transform.setOrigin(btVector3(-stadion_dimensions[0],stadion_dimensions[1]/2,0));
	stadion->addChildShape(stadion_transform,left);
	stadion_transform.setOrigin(btVector3(stadion_dimensions[0],stadion_dimensions[1]/2,0));
	stadion->addChildShape(stadion_transform,right);
	stadion_transform.setOrigin(btVector3(0,stadion_dimensions[1]/2,stadion_dimensions[2]));
	stadion->addChildShape(stadion_transform,front);
	stadion_transform.setOrigin(btVector3(0,stadion_dimensions[1]/2,-stadion_dimensions[2]));
	stadion->addChildShape(stadion_transform,back);

	stadion_transform.setOrigin(btVector3(0,0,0));

	btRigidBody* staticStadion = m_coloredWorld->addRigidBody(0, stadion_transform,stadion,COL_STRUCTURES,STRUCTURES_COL_WITH, btVector3(1.f,0.7f,0.4f));
	staticStadion->setCollisionFlags(staticStadion->getCollisionFlags()|btCollisionObject::CF_STATIC_OBJECT);
	staticStadion->setCollisionFlags(staticStadion->getCollisionFlags()|btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

	m_coloredWorld->addTexture(staticStadion, "grass.jpg");


	// Create the boundary behind the player
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(0,0,50));
	m_coloredWorld->addRigidBody(100000000000000000,trans,new btBoxShape(btVector3(200,200,2)),COL_BOUNDARY,BOUNDARY_COL_WITH,
			btVector3(1,0,0))->setGravity(btVector3(0,0,0));
	trans.setOrigin(btVector3(-62,0,0));
	m_coloredWorld->addRigidBody(100000000000000000,trans,new btBoxShape(btVector3(2,200,200)),COL_BOUNDARY,BOUNDARY_COL_WITH,
				btVector3(1,0,0))->setGravity(btVector3(0,0,0));
	trans.setOrigin(btVector3(62,0,0));
	m_coloredWorld->addRigidBody(100000000000000000,trans,new btBoxShape(btVector3(2,200,200)),COL_BOUNDARY,BOUNDARY_COL_WITH,
				btVector3(1,0,0))->setGravity(btVector3(0,0,0));
	trans.setOrigin(btVector3(0,0,-200));
	m_coloredWorld->addRigidBody(100000000000000000,trans,new btBoxShape(btVector3(200,200,2)),COL_BOUNDARY,BOUNDARY_COL_WITH,
				btVector3(1,0,0))->setGravity(btVector3(0,0,0));
	trans.setOrigin(btVector3(0,200,0));
	m_coloredWorld->addRigidBody(100000000000000000,trans,new btBoxShape(btVector3(200,2,200)),COL_BOUNDARY,BOUNDARY_COL_WITH,
				btVector3(1,0,0))->setGravity(btVector3(0,0,0));


}

void	Field::clientResetScene()
{
	exitPhysics();
	initPhysics();
}
	
void Field::startGame(ArcadeConfiguration::difficulties difficulty, ArcadeConfiguration::gameMode mode){


	if(mode == ArcadeConfiguration::MODE_ARCADE){
		m_currentMode = ArcadeConfiguration::MODE_ARCADE;
		ArcadeConfiguration* config;
		string loadString;
		bool setCastles= false;
		switch(difficulty){

		case ArcadeConfiguration::DIFF_EASY :
			m_currentDifficulty = ArcadeConfiguration::DIFF_EASY;
			loadString = "../Levels/Easy.xml";
			config = m_configLoader.readConfigFile(loadString);
			m_sounds->setDifficulty(ArcadeConfiguration::DIFF_EASY);
			break;
		case ArcadeConfiguration::DIFF_MEDIUM :
			m_currentDifficulty = ArcadeConfiguration::DIFF_MEDIUM;
			loadString = "../Levels/Medium.xml";
			config = m_configLoader.readConfigFile(loadString);
			m_sounds->setDifficulty(ArcadeConfiguration::DIFF_MEDIUM);
			break;
		case ArcadeConfiguration::DIFF_HARD :
			m_currentDifficulty = ArcadeConfiguration::DIFF_HARD;
			loadString = "../Levels/Hard.xml";
			config = m_configLoader.readConfigFile(loadString);
			m_sounds->setDifficulty(ArcadeConfiguration::DIFF_HARD);
			break;
		case ArcadeConfiguration::DIFF_MADNESS :
			m_currentDifficulty = ArcadeConfiguration::DIFF_MADNESS;
			loadString = "../Levels/Madness.xml";
			config = m_configLoader.readConfigFile(loadString);
			m_sounds->setDifficulty(ArcadeConfiguration::DIFF_MADNESS);
			break;
		case ArcadeConfiguration::DIFF_TRAINING :
			m_currentDifficulty = ArcadeConfiguration::DIFF_TRAINING;
			loadString = "../Levels/Training.xml";
			config = m_configLoader.readConfigFile(loadString);

			// CASTLES :)
			setCastles = true;
			break;
		default :
			break;
		}

		if(setCastles){
			Castle* cas = new Castle(20,-100);
			Castle* cas2 = new Castle(-20,-100);
			Castle* cas3 = new Castle(30,-130);
			Castle* cas4 = new Castle(-30,-130);
			castleList.push_back(cas);
			castleList.push_back(cas2);
			castleList.push_back(cas3);
			castleList.push_back(cas4);
		}

		m_canon->setConfiguration(config);
		m_canon->updateLevelData(1); // start bei Level 1 ??

		m_levelHandler->setArcadeConfiguration(config);
		m_levelHandler->setCurrentLevel(1);
		m_levelHandler->setMaxLevels(config->getLevelList().size());
		m_levelHandler->initialize();


	}

	if(mode == ArcadeConfiguration::MODE_SPARTA){
		m_currentMode = ArcadeConfiguration::MODE_SPARTA;
		SpartaConfiguration* config;
		string loadString;
		switch(difficulty){

		case ArcadeConfiguration::DIFF_EASY :
			loadString = "../Levels/SP_Easy.xml";
			config = m_configLoader.readSpartaFile(loadString);
			m_currentDifficulty = ArcadeConfiguration::DIFF_EASY;
			m_sounds->setDifficulty(ArcadeConfiguration::DIFF_EASY);
			break;
		case ArcadeConfiguration::DIFF_MEDIUM :
			loadString = "../Levels/SP_Medium.xml";
			config = m_configLoader.readSpartaFile(loadString);
			m_currentDifficulty = ArcadeConfiguration::DIFF_MEDIUM;
			m_sounds->setDifficulty(ArcadeConfiguration::DIFF_MEDIUM);
			break;
		case ArcadeConfiguration::DIFF_HARD :
			loadString = "../Levels/SP_Hard.xml";
			config = m_configLoader.readSpartaFile(loadString);
			m_currentDifficulty = ArcadeConfiguration::DIFF_HARD;
			m_sounds->setDifficulty(ArcadeConfiguration::DIFF_HARD);
			break;
		case ArcadeConfiguration::DIFF_MADNESS :
			loadString = "../Levels/SP_Madness.xml";
			config = m_configLoader.readSpartaFile(loadString);
			m_currentDifficulty = ArcadeConfiguration::DIFF_MADNESS;
			m_sounds->setDifficulty(ArcadeConfiguration::DIFF_MADNESS);
			break;

		default :
			break;
		}

		m_spartaCannon->setConfiguration(config);
		m_spartaCannon->updateLevelData(1); // start bei Level 1 ??
		m_spartaCannon->start();

		m_levelHandler->setSpartaConfiguration(config);
		m_levelHandler->setCurrentLevel(1);
		m_levelHandler->setMaxLevels(config->getLevelList().size());
		m_levelHandler->initialize();

	}

	m_menu->closeMenu();
//	m_gameStarted = true;
	m_menu->invokeMenu(MenuList::GO_MENU);

	Message sendmsg;
	sendmsg.messageType = Message::MSG_TOGGLE_EXPLANATION;
	sendMessage(sendmsg);

}

void	Field::exitPhysics()
{

	//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them
	m_coloredWorld->cleanup();

	//delete collision shapes
	for (int j=0;j<m_collisionShapes.size();j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
	m_collisionShapes.clear();

	delete m_coloredWorld;
	
	delete m_solver;
	
	delete m_broadphase;
	
	delete m_dispatcher;

	delete m_collisionConfiguration;

	delete m_canon;
	
	m_castlePointCount->destroy();

	m_pointing->destroy();

}

