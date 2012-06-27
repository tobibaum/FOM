/*
 * Hendrik.cpp
 *
 *  Created on: May 23, 2011
 *      Author: Tobi, Hendrik
 */

#include "Hendrik.h"

Hendrik::Hendrik() {
	// TODO Auto-generated constructor stub

}

Hendrik::~Hendrik() {
	// TODO Auto-generated destructor stub
}

void Hendrik::step(){
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


}

void Hendrik::initWorld(){
//	btTransform startTrans;
//	startTrans.setIdentity();
//	startTrans.setOrigin(btVector3(0,0,-100));
//
//	m_coloredWorld->addWallObject(startTrans,"easy2.png",5,40);

	m_showFOM = false;
//	srand(time(0));
	// The order is important here! Draw the cannon first, s.t. transparency is possible!
		// Player(s) have to be added first, the rendering order depends on this.

//		m_menu->createMainMenu();

		//createMenuPoints();

//		m_castle = new Castle(m_coloredWorld);
//		m_castle->create(-10,-28);

	//	thePlayer->createPoints(m_dynamicsWorld);

//		//	m_canon->set_shooting_direction(btVector3(0,1.5,1));
//		m_canon->set_target(btVector3(230,2,0));
//		m_canon->setFrequency(1.4);
//		m_canon->setSpeed(140);

//		m_menu = new MenuList(m_coloredWorld);
//		m_menu->setPlayerPosition(new btVector3(5,0,0));
//		m_menu->createMainMenu();
//		m_menu->setRotationCalled(true);

//		rotateTest = 0;

//		btCollisionShape* shape = m_shapeFac.getCakeWithHeight(btVector3(0,0,0), btVector3(5,0,0), btVector3(2.5,5,0),50);
//		btTransform trans;
//		trans.setIdentity();
//		trans.setOrigin(btVector3(-15,40,-105));
//		trans.setRotation(btQuaternion(3.141592653/2,3.141592653,0));
//		btRigidBody* wall = m_coloredWorld->addRigidBody(0,trans,shape,0,0,btVector3(1,1,1),0);
//		m_coloredWorld->addTexture(wall, "beton.bmp");

//		std::string load = "Easy.xml";
//		ArcadeConfiguration* a = m_configLoader.readConfigFile(load);
//		printf("data : %i \n", a->getLevelList().front()->getBallThresh());
//		m_canon->setBulletShape(shape);

//		printf("\n %f\n",config->getShootSpeed(1));
//		delete m_confLoader;

//		m_highscoreHandler = new HighscoreHandler();
//		m_highscoreHandler->addToHighscoreList("blu",1);
		//bla

}
