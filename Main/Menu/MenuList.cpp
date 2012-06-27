/*
 * MenuList.cpp
 *
 *  Created on: May 17, 2011
 *      Author: hendrik
 */

#include "MenuList.h"

using namespace std;

#define PI 3.1415926535897932384626433832795028841971

MenuList::MenuList(ColoredWorld* ownerWorld) {
	m_ownerWorld = ownerWorld;
	currentState = MAIN_MENU;
	m_rotationCalled = false;
	m_menuChangeCalled = false;
	m_passed_in_rotation = 0;
	m_time_for_one_rotation = 1000;
	m_bufferTime = 0;
	m_absDistance = 0;
	m_last_time = m_clock.getTimeMilliseconds();
	m_skeleton = Skeleton::getInstance();

	m_playerPosition = m_skeleton->getSkeleton().head;

	m_bufferTimeThreshold = 1000;

	m_frontPosition = new btVector3(0,0,0);
	initMap();

// Get messages initialization
	setTrigger(Message::MSG_COL_PLAYER_LETTER);
	setTrigger(Message::MSG_CURRENT_DIFFICULTY);
	setTrigger(Message::MSG_PAUSE_GAME);

	/*****************/
	m_nextMenu = MAIN_MENU;
//	setMenuChangeCalled(true);
//	m_rotation_direction = ROT_LEFT;
	m_highscoreBody = 0;
	m_highscoreShape = 0;

	m_arcade_selected = false;
	m_highscore_selected = false;

	m_currentDiff = ArcadeConfiguration::DIFF_EASY;
}

bool MenuList::getMenuChangeCalled() const
{
return m_menuChangeCalled;
}

void MenuList::setMenuChangeCalled(bool m_menuChangeCalled)
{
this->m_menuChangeCalled = m_menuChangeCalled;
btScalar this_time = m_clock.getTimeMilliseconds();
m_bufferTime = this_time;
m_bufferTimeThreshold = this_time + 1000;
}

void MenuList::initMap(){
	menuMap["PLAY GAME"] = SEL_PLAY_GAME;
	menuMap["EXIT"] = SEL_EXIT_CALLED;
	menuMap["HIGHSCORES"] = SEL_HIGHSCORES;
	menuMap["BACK"] = SEL_BACK;
	menuMap["ARCADE"] = SEL_ARCADE;
	menuMap["HIT TRAINING"] = SEL_HIT_TRAINING;
	menuMap["EASY"] = SEL_EASY;
	menuMap["MEDIUM"] = SEL_MEDIUM;
	menuMap["HARD"] = SEL_HARD;
	menuMap["MADNESS"] = SEL_MADNESS;
	menuMap["ARCADE"] = SEL_ARCADE;
	menuMap["SPARTA"] = SEL_SPARTA;
	menuMap["<"] = SEL_ROTATE_LEFT;
	menuMap[">"] = SEL_ROTATE_RIGHT;
	menuMap["RETURN"] = SEL_RETURN;
	menuMap["END"] = SEL_END;
	menuMap["GOAL REACHED"] = SEL_WON;
	menuMap["GO"] = SEL_GO;
	menuMap["INSTRUCTIONS"] = SEL_INSTRUCTIONS;
	menuMap["SAVE HIGHSCORE"] = SEL_SAVE_HS;
	menuMap["MAIN MENU"] = SEL_WON;
	menuMap["NEXT LEVEL"] = SEL_NEXT_LEVEL;
	menuMap["CHANGE PLAYER"] = SEL_CHANGE_PLAYER;
	menuMap["ENTER NAME"] = SEL_HIGHSCORE_ENTERED;
	menuMap["CORRECT"] = SEL_CORRECT;
	menuMap["A"] = SEL_SINGLE_LETTER;	menuMap["N"] = SEL_SINGLE_LETTER;
	menuMap["B"] = SEL_SINGLE_LETTER;	menuMap["O"] = SEL_SINGLE_LETTER;
	menuMap["C"] = SEL_SINGLE_LETTER;	menuMap["P"] = SEL_SINGLE_LETTER;
	menuMap["D"] = SEL_SINGLE_LETTER;	menuMap["Q"] = SEL_SINGLE_LETTER;
	menuMap["E"] = SEL_SINGLE_LETTER;	menuMap["R"] = SEL_SINGLE_LETTER;
	menuMap["F"] = SEL_SINGLE_LETTER;	menuMap["S"] = SEL_SINGLE_LETTER;
	menuMap["G"] = SEL_SINGLE_LETTER;	menuMap["T"] = SEL_SINGLE_LETTER;
	menuMap["H"] = SEL_SINGLE_LETTER;	menuMap["U"] = SEL_SINGLE_LETTER;
	menuMap["I"] = SEL_SINGLE_LETTER;	menuMap["V"] = SEL_SINGLE_LETTER;
	menuMap["J"] = SEL_SINGLE_LETTER;	menuMap["W"] = SEL_SINGLE_LETTER;
	menuMap["K"] = SEL_SINGLE_LETTER;	menuMap["X"] = SEL_SINGLE_LETTER;
	menuMap["L"] = SEL_SINGLE_LETTER;	menuMap["Y"] = SEL_SINGLE_LETTER;
	menuMap["M"] = SEL_SINGLE_LETTER;	menuMap["Z"] = SEL_SINGLE_LETTER;
}

MenuList::~MenuList() {
	closeMenu();
}

void MenuList::createNavigationArrows(){
	btTransform trans;
	trans.setIdentity();

	//Rightarrow
	trans.setOrigin(btVector3(14,15,-3));
	m_Rnav_shape = m_letterDrawer.getLetters(">");
	btRigidBody* add = m_ownerWorld->addRigidBody(MENUPOINT_MASS,trans,m_Rnav_shape,COL_LETTERS, LETTERS_COL_WITH, btVector3(0.8f,0.3f,0.2f));
	add->setGravity(btVector3(0,0,0));
	add->setActivationState(DISABLE_DEACTIVATION);
	m_Rnav_body = add;

	// Leftarrow
	trans.setOrigin(btVector3(-14,15,-3));
	m_Lnav_shape = m_letterDrawer.getLetters("<");
	btRigidBody* add2 = m_ownerWorld->addRigidBody(MENUPOINT_MASS,trans,m_Lnav_shape,COL_LETTERS, LETTERS_COL_WITH, btVector3(0.8f,0.3f,0.2f));
	add2->setGravity(btVector3(0,0,0));
	add2->setActivationState(DISABLE_DEACTIVATION);
	m_Lnav_body = add2;

	m_arrowsDeleted = false;

}

void MenuList::deleteNavigationArrows(){
	m_ownerWorld->deleteRigidBody(m_Lnav_body);
	delete m_Lnav_shape;

	m_ownerWorld->deleteRigidBody(m_Rnav_body);
	delete m_Rnav_shape;

	m_arrowsDeleted = true;
}

bool MenuList::getRotationCalled() const{
    return m_rotationCalled;
}

void MenuList::setRotationCalled(bool rotationCalled){
	deleteNavigationArrows();
	setBackFront();
    this->m_rotationCalled = rotationCalled;
//    m_time_since_last_rot = m_clock.getTimeMilliseconds();
    m_passed_in_rotation = 0;
}


void MenuList::clearMenuPoints(){
	list<btRigidBody*>::iterator bodyIt;
	list<btCompoundShape*>::iterator shapeIt;
	list<string>::iterator stringIt;

	bodyIt = currentBodies.begin();
	shapeIt = currentCShapes.begin();
	stringIt = actualMenuPoints.begin();

	while(stringIt != actualMenuPoints.end()){
		m_ownerWorld->deleteRigidBody(*bodyIt);
		delete (*shapeIt);
		bodyIt++;
		shapeIt++;
		stringIt++;
	}

	currentBodies.clear();
	currentCShapes.clear();
	actualMenuPoints.clear();

	if(!m_arrowsDeleted){
		deleteNavigationArrows();
	}

}

void MenuList::trigger(Message msg){
	if(msg.messageType == Message::MSG_CURRENT_DIFFICULTY){
		m_currentDiff = msg.additionalInfo;
	}
	else if(msg.messageType == Message::MSG_PAUSE_GAME){
		if(actualMenuPoints.size() > 0){
			clearMenuPoints();
			setMenuChangeCalled(true);
		}
	}
	else{
	string type = msg.menuPointWord;
	int menType = menuMap.at(type);
	string newname="";
	Message sendmsg;
//	printf("triggered\n");
	switch(menType){

		case SEL_CHANGE_PLAYER :
			 	 	 	 	 	 	   sendmsg.additionalInfo = 0;
									   sendmsg.messageType = Message::MSG_STOP_KINECT;
									   sendMessage(sendmsg);
									   closeMenu();
			break;

		case SEL_PLAY_GAME : m_nextMenu = MODE_MENU;
						 setMenuChangeCalled(true);

			break;
		case SEL_HIGHSCORES :
								m_nextMenu = HIGHSCORE_SELECT_MENU;
								setMenuChangeCalled(true);
								m_highscore_selected = true;
//							   m_nextMenu = HIGHSCORE_WATCH_MENU;
//							   setMenuChangeCalled(true);
//							   sendmsg.additionalInfo = 0;
//							   sendmsg.messageType = Message::MSG_HIGHSCORE_CALL;
//							   sendMessage(sendmsg);

			break;
		case SEL_EXIT_CALLED :
			   clearMenuPoints();
//						   deleteNavigationArrows();
			   sendmsg.additionalInfo = 0;
			   sendmsg.messageType = Message::MSG_EXIT_CALL;
			   sendMessage(sendmsg);

			break;

		case SEL_INSTRUCTIONS :
			m_nextMenu = INSTRUCTIONS_MENU;
		    setMenuChangeCalled(true);
			sendmsg.additionalInfo = 0;
		    sendmsg.messageType = Message::MSG_INSTRUCTIONS;
		    sendMessage(sendmsg);

			break;
		case SEL_ARCADE :
					  if(!m_highscore_selected){
						  m_nextMenu = DIFFICULTY_MENU;
						  setMenuChangeCalled(true);
						  m_arcade_selected = true;
					  }
					  else{
						  m_nextMenu = HIGHSCORE_WATCH_MENU;
						   setMenuChangeCalled(true);
						   sendmsg.messageType = Message::MSG_HIGHSCORE_DIFF_SELECTED;
						   sendmsg.stringInfo = "EASY";
						   sendMessage(sendmsg);
						   sendmsg.additionalInfo = HS_CALL_ARCADE;
						   sendmsg.messageType = Message::MSG_HIGHSCORE_CALL;
						   sendMessage(sendmsg);
						   //nachricht über actuelle Diff
//						   sendmsg.messageType = Message::MSG_HIGHSCORE_DIFF_SELECTED;
//						   sendmsg.stringInfo = ((LetterCompoundShape*)(this->m_frontBody->getCollisionShape()))->getType();

					  }
			break;
		case SEL_SPARTA :
							if(!m_highscore_selected){
							  m_nextMenu = DIFFICULTY_MENU;
							  setMenuChangeCalled(true);
							  m_arcade_selected = false;
							}
							else{
								  m_nextMenu = HIGHSCORE_WATCH_MENU;
								   setMenuChangeCalled(true);
								   //Reihenfolge hier vermutlich nciht egal
								   sendmsg.messageType = Message::MSG_HIGHSCORE_DIFF_SELECTED;
								   sendmsg.stringInfo = "EASY";
								   sendMessage(sendmsg);
								   sendmsg.additionalInfo = HS_CALL_SPARTA;
								   sendmsg.messageType = Message::MSG_HIGHSCORE_CALL;
								   sendMessage(sendmsg);

							  }
			break;
		case SEL_BACK : /*printf("BACK recognized\n");*/
					switch(m_nextMenu){
						case MODE_MENU : m_nextMenu = MAIN_MENU;
							setMenuChangeCalled(true);
//							printf("2nd lvl switch endtered\n");
							break;
						case DIFFICULTY_MENU : m_nextMenu = MODE_MENU;
							setMenuChangeCalled(true);
//							printf("2nd lvl switch endtered wrong\n");
							break;
						case HIGHSCORE_SELECT_MENU : m_nextMenu = MAIN_MENU;
							 setMenuChangeCalled(true);
							 m_highscore_selected = false;
							 break;
						case INSTRUCTIONS_MENU : m_nextMenu = MAIN_MENU;
							setMenuChangeCalled(true);
							sendmsg.additionalInfo = 0;
							sendmsg.messageType = Message::MSG_INSTRUCTIONS;
							sendMessage(sendmsg);
							break;
						case HIGHSCORE_WATCH_MENU : m_nextMenu = MAIN_MENU;
							setMenuChangeCalled(true);
							m_highscore_selected = false;
//							printf("2nd lvl switch endtered wrong\n");
//							sendmsg.additionalInfo = 0;
//						    sendmsg.messageType = Message::MSG_HIGHSCORE_CALL;
//						    sendMessage(sendmsg);
							break;
						default : /*printf("i shouldnt write this");*/ break;
					}
			break;
		case SEL_GO:    sendmsg.additionalInfo = 0;
						sendmsg.messageType = Message::MSG_GAME_GO;
						sendMessage(sendmsg);
						clearMenuPoints();
			break;
		case SEL_HIT_TRAINING :    sendmsg.additionalInfo = ArcadeConfiguration::DIFF_TRAINING;
								   sendmsg.gameMode = ArcadeConfiguration::MODE_ARCADE;
								   sendmsg.messageType = Message::MSG_START_GAME;
								   sendMessage(sendmsg);
			break;
		case SEL_EASY :    sendmsg.additionalInfo = ArcadeConfiguration::DIFF_EASY;
						   if(m_arcade_selected){
							   sendmsg.gameMode = ArcadeConfiguration::MODE_ARCADE;
						   }else{sendmsg.gameMode = ArcadeConfiguration::MODE_SPARTA;}
						   sendmsg.messageType = Message::MSG_START_GAME;
						   sendMessage(sendmsg);
			break;
		case SEL_MEDIUM :  sendmsg.additionalInfo = ArcadeConfiguration::DIFF_MEDIUM;
						   if(m_arcade_selected){
							   sendmsg.gameMode = ArcadeConfiguration::MODE_ARCADE;
						   }else{sendmsg.gameMode = ArcadeConfiguration::MODE_SPARTA;}
						   sendmsg.messageType = Message::MSG_START_GAME;
						   sendMessage(sendmsg);
				break;
		case SEL_HARD :    sendmsg.additionalInfo = ArcadeConfiguration::DIFF_HARD;
						   if(m_arcade_selected){
							   sendmsg.gameMode = ArcadeConfiguration::MODE_ARCADE;
						   }else{sendmsg.gameMode = ArcadeConfiguration::MODE_SPARTA;}
						   sendmsg.messageType = Message::MSG_START_GAME;
						   sendMessage(sendmsg);
			break;
		case SEL_MADNESS : sendmsg.additionalInfo = ArcadeConfiguration::DIFF_MADNESS;
						   if(m_arcade_selected){
							   sendmsg.gameMode = ArcadeConfiguration::MODE_ARCADE;
						   }else{sendmsg.gameMode = ArcadeConfiguration::MODE_SPARTA;}
						   sendmsg.messageType = Message::MSG_START_GAME;
						   sendMessage(sendmsg);
			break;

		case SEL_ROTATE_LEFT :
							if(m_frontBody->getWorldTransform().getOrigin().distance(btVector3(0,20,-12)) == 0){
								this->setRotationCalled(true);
								m_rotation_direction = ROT_LEFT;
							}
			break;
		case SEL_ROTATE_RIGHT :
							if(m_frontBody->getWorldTransform().getOrigin().distance(btVector3(0,20,-12)) == 0){
								this->setRotationCalled(true);
								m_rotation_direction = ROT_RIGHT;
							}
			break;

		case SEL_RETURN :  sendmsg.additionalInfo = 0;
						   sendmsg.messageType = Message::MSG_RETURN_GAME;
						   sendMessage(sendmsg);
						   clearMenuPoints();
			break;

		case SEL_END :     sendmsg.additionalInfo = 0;
						   sendmsg.messageType = Message::MSG_END_GAME_WITHOUT_HS;
						   sendMessage(sendmsg);
			break;
		case SEL_SAVE_HS :     sendmsg.additionalInfo = 0;
						   sendmsg.messageType = Message::MSG_END_GAME;
						   sendMessage(sendmsg);
			break;
		case SEL_WON:	   sendmsg.additionalInfo = 0;
						   sendmsg.messageType = Message::MSG_END_GAME;
						   sendMessage(sendmsg);
			break;

		case SEL_NEXT_LEVEL:   sendmsg.additionalInfo = 0;
							   sendmsg.messageType = Message::MSG_NEXT_LEVEL;
							   sendMessage(sendmsg);

			break;
		case SEL_HIGHSCORE_ENTERED:
								   sendmsg.additionalInfo = 0;
								   sendmsg.messageType = Message::MSG_NAME_ENTERED;
								   sendmsg.stringInfo = m_highscoreName;
								   sendMessage(sendmsg);
								   m_highscoreName = "";
								   if(m_highscoreBody){m_ownerWorld->deleteRigidBody(m_highscoreBody);}
								   if(m_highscoreShape){delete m_highscoreShape;}
								   m_highscoreBody = 0;
								   m_highscoreShape = 0;
			break;

		case SEL_SINGLE_LETTER:
							   m_highscoreName = m_highscoreName+msg.menuPointWord;
							   createHighscoreName();
							   invokeMenu(HIGHSCORE_MENU);
			break;

		case SEL_CORRECT:
							   if(m_highscoreName.size() > 0){
								   for(int i=0; i<(m_highscoreName.size()-1); i++){
									   newname = newname + m_highscoreName.at(i);
								   }
								   m_highscoreName = newname;
								   createHighscoreName();
							   }
							   invokeMenu(HIGHSCORE_MENU);
			break;

		default: break;

	}
	}
}

void MenuList::process(float ms){
	if(this->m_rotationCalled){
		rotate(ms);
	}

	if(m_menuChangeCalled){
		if(m_bufferTime > m_bufferTimeThreshold){
			user usr = m_skeleton->getSkeleton();
			btVector3 headPos = btVector3(usr.head.getX(),0,usr.head.getZ());
			if(headPos.distance(btVector3(0,0,0)) < MENU_TRIGGER_RANGE){
				m_menuChangeCalled = false;
				clearMenuPoints();
		//			printf("menu cleared\n");
				createNextMenu();
		//			printf("next menu created\n");
				m_menuChangeCalled = false;
			}
		}
		else{
			m_bufferTime += this->get_time_delta_in_ms(m_bufferTime);
		}
	}

}

//void MenuList::moveInOut(btScalar step){
//	m_absDistance = m_absDistance + step;
//	printf("%f\n", m_absDistance);
//	btVector3 origin = m_frontBody->getWorldTransform().getOrigin();
//	m_frontBody->getWorldTransform().setOrigin(origin - btVector3(0,0,m_absDistance));
//
//	origin = m_nextBody->getWorldTransform().getOrigin();
//	m_nextBody->getWorldTransform().setOrigin(origin + btVector3(0,0,m_absDistance));
//}
//
//void MenuList::moveInOutUndo(){
//	btVector3 origin = m_frontBody->getWorldTransform().getOrigin();
//	m_frontBody->getWorldTransform().setOrigin(origin + btVector3(0,0,m_absDistance));
//
//	origin = m_nextBody->getWorldTransform().getOrigin();
//	m_nextBody->getWorldTransform().setOrigin(origin - btVector3(0,0,m_absDistance));
//}

void MenuList::rotate(float ms){
	if(m_nextMenu == MenuList::HIGHSCORE_MENU){
		m_time_for_one_rotation = 500;
	} else{m_time_for_one_rotation = 1000;}
	if(m_passed_in_rotation < m_time_for_one_rotation){


//		if(m_time_since_last_rot > ms){
		int itemCount = actualMenuPoints.size();
		float angle = ((2*PI)/itemCount) * (ms/m_time_for_one_rotation);
//			moveInOutUndo();


		list<btRigidBody*>::iterator it;
		for(it = currentBodies.begin();it!=currentBodies.end(); it++){
			btTransform trans;
			trans.setIdentity();
			btMatrix3x3* rotationMat = new btMatrix3x3(cos(angle),0,sin(angle),
														   0     ,1,    0     ,
													  -sin(angle),0,cos(angle));

			if(m_rotation_direction == ROT_RIGHT){
				*rotationMat = rotationMat->transpose();
			}
			trans.setOrigin(((*rotationMat)*(((*it)->getWorldTransform()).getOrigin()) - m_playerPosition) + m_playerPosition);
			float objAngle;
			btVector3 projected = trans.getOrigin();
			projected.setY(0);
			if(trans.getOrigin().getX() > 0){
				objAngle = 2*PI - (projected.angle(btVector3(0,0,-1)));
				trans.setRotation(btQuaternion(btVector3(0.f,1.f,0.f),objAngle));
			}
			else{
				objAngle = projected.angle(btVector3(0,0,-1));
				trans.setRotation(btQuaternion(btVector3(0.f,1.f,0.f),objAngle));
			}
			(*it)->getWorldTransform() = trans;

		}
		m_passed_in_rotation += ms;

//			moveInOut(m_frontDistance/m_steps);

//		} else {
//			m_time_since_last_rot += ms;
//		}
	}
	else{
		m_passed_in_rotation = 0;

		m_rotationCalled = false;
		createNavigationArrows();
//		m_absDistance = 0;

		// reorder list
		if(m_rotation_direction == ROT_RIGHT){
			btRigidBody* front = currentBodies.front();
			currentBodies.pop_front();
			currentBodies.push_back(front);
			m_frontBody = currentBodies.front();
			delete m_frontPosition;
			m_frontPosition = new btVector3(currentBodies.front()->getWorldTransform().getOrigin().getX(),
											currentBodies.front()->getWorldTransform().getOrigin().getY(),
											currentBodies.front()->getWorldTransform().getOrigin().getZ());
		}
		if(m_rotation_direction == ROT_LEFT){
			btRigidBody* back = currentBodies.back();
			currentBodies.pop_back();
			currentBodies.push_front(back);
			m_frontBody = currentBodies.front();
			delete m_frontPosition;
			m_frontPosition = new btVector3(currentBodies.front()->getWorldTransform().getOrigin().getX(),
											currentBodies.front()->getWorldTransform().getOrigin().getY(),
											currentBodies.front()->getWorldTransform().getOrigin().getZ());
		}
		setFront();
		if(m_nextMenu == HIGHSCORE_WATCH_MENU){
			Message sendmsg;
			sendmsg.messageType = Message::MSG_HIGHSCORE_DIFF_SELECTED;
			sendmsg.stringInfo = ((LetterCompoundShape*)(this->m_frontBody->getCollisionShape()))->getType();
//			if(sendmsg.stringInfo.compare("BACK") != 0){
				sendMessage(sendmsg);
//			}
		}
	}

}

void MenuList::createNextMenu(){
//	printf("creating next Menu\n");
	switch(m_nextMenu){
		case MAIN_MENU : createMainMenu();
			break;
		case DIFFICULTY_MENU : createDifficultyMenu();
			break;
		case MODE_MENU : createPlayGameMenu();
			break;
		case HIGHSCORE_MENU : createHighscoreMenu();
			break;
		case HIGHSCORE_WATCH_MENU : createHighscoreWatchMenu();
			break;
		case HIGHSCORE_SELECT_MENU : createHighscoreSelectMenu();
			break;
		case STATISTICS_MENU : createStatisticsMenu();
			break;
		case GO_MENU : createGoMenu();
			break;
		case INSTRUCTIONS_MENU : createInstructionsMenu();
			break;
		case PAUSE_MENU : createPauseMenu();
			break;
		default :
			break;
	}
}

void MenuList::createMainMenu(){
	actualMenuPoints.push_back("PLAY GAME");
	actualMenuPoints.push_back("HIGHSCORES");
	actualMenuPoints.push_back("CHANGE PLAYER");
	actualMenuPoints.push_back("INSTRUCTIONS");
	actualMenuPoints.push_back("EXIT");

	createMenuPoints();

}

void MenuList::createHighscoreWatchMenu(){
	switch(m_currentDiff){
	case ArcadeConfiguration::DIFF_EASY :
		actualMenuPoints.push_back("EASY");
		actualMenuPoints.push_back("MEDIUM");
		actualMenuPoints.push_back("HARD");
		actualMenuPoints.push_back("MADNESS");
		actualMenuPoints.push_back("BACK");
		break;
	case ArcadeConfiguration::DIFF_MEDIUM :
		actualMenuPoints.push_back("MEDIUM");
		actualMenuPoints.push_back("HARD");
		actualMenuPoints.push_back("MADNESS");
		actualMenuPoints.push_back("BACK");
		actualMenuPoints.push_back("EASY");
		break;
	case ArcadeConfiguration::DIFF_HARD :
		actualMenuPoints.push_back("HARD");
		actualMenuPoints.push_back("MADNESS");
		actualMenuPoints.push_back("BACK");
		actualMenuPoints.push_back("EASY");
		actualMenuPoints.push_back("MEDIUM");
		break;
	case ArcadeConfiguration::DIFF_MADNESS :
		actualMenuPoints.push_back("MADNESS");
		actualMenuPoints.push_back("BACK");
		actualMenuPoints.push_back("EASY");
		actualMenuPoints.push_back("MEDIUM");
		actualMenuPoints.push_back("HARD");
		break;

	}


	createMenuPoints();

}

void MenuList::createHighscoreSelectMenu(){
	actualMenuPoints.push_back("ARCADE");
	actualMenuPoints.push_back("SPARTA");
	actualMenuPoints.push_back("BACK");

	createMenuPoints();

}

void MenuList::createGoMenu(){
	actualMenuPoints.push_back("GO");

	createMenuPoints();

	deleteNavigationArrows();

}

void MenuList::createInstructionsMenu(){
	actualMenuPoints.push_back("BACK");

	createMenuPoints();

	deleteNavigationArrows();

}

void MenuList::createStatisticsMenu(){
	actualMenuPoints.push_back("NEXT LEVEL");
	actualMenuPoints.push_back("MAIN MENU");

	createMenuPoints();

}

void MenuList::createHighscoreMenu(){
	actualMenuPoints.push_back("A");
	actualMenuPoints.push_back("B");
	actualMenuPoints.push_back("C");
	actualMenuPoints.push_back("D");
	actualMenuPoints.push_back("E");
	actualMenuPoints.push_back("F");
	actualMenuPoints.push_back("G");
	actualMenuPoints.push_back("H");
	actualMenuPoints.push_back("I");
	actualMenuPoints.push_back("J");
	actualMenuPoints.push_back("K");
	actualMenuPoints.push_back("L");
	actualMenuPoints.push_back("M");
	actualMenuPoints.push_back("N");
	actualMenuPoints.push_back("O");
	actualMenuPoints.push_back("P");
	actualMenuPoints.push_back("Q");
	actualMenuPoints.push_back("R");
	actualMenuPoints.push_back("S");
	actualMenuPoints.push_back("T");
	actualMenuPoints.push_back("U");
	actualMenuPoints.push_back("V");
	actualMenuPoints.push_back("W");
	actualMenuPoints.push_back("X");
	actualMenuPoints.push_back("Y");
	actualMenuPoints.push_back("Z");
	actualMenuPoints.push_back("CORRECT");
	actualMenuPoints.push_back("ENTER NAME");

	createMenuPoints();

}

void MenuList::createPauseMenu(){
	actualMenuPoints.push_back("RETURN");
	actualMenuPoints.push_back("END");
	actualMenuPoints.push_back("SAVE HIGHSCORE");

	createMenuPoints();

}

void MenuList::createWonMenu(){

	actualMenuPoints.push_back("GOAL REACHED");

	createMenuPoints();

	deleteNavigationArrows();
}

void MenuList::createEndMenu(){

	actualMenuPoints.push_back("THE END");

	createMenuPoints();

	deleteNavigationArrows();

}

void MenuList::createPlayGameMenu(){
	actualMenuPoints.push_back("ARCADE");
	actualMenuPoints.push_back("SPARTA");
	actualMenuPoints.push_back("HIT TRAINING");
	actualMenuPoints.push_back("BACK");


	createMenuPoints();
}

void MenuList::createDifficultyMenu(){
	actualMenuPoints.push_back("EASY");
	actualMenuPoints.push_back("MEDIUM");
	actualMenuPoints.push_back("HARD");
	actualMenuPoints.push_back("MADNESS");
	actualMenuPoints.push_back("BACK");

	createMenuPoints();
}

void MenuList::createMenuPoints(){


	btTransform lightTrans;
	lightTrans.setOrigin(btVector3(0,150,0));

	user usr = m_skeleton->getSkeleton();
	m_playerPosition = btVector3(0,0,0);

	int itemCount = actualMenuPoints.size();
	list<string>::iterator it;
	int i=0;

	// Create a Shape for each Menu Point and place it right
	for(it = actualMenuPoints.begin(); it != actualMenuPoints.end(); it++){

		btTransform trans;
		trans.setIdentity();
		btVector3 offset(0,20,-(itemCount*8));

		trans.setOrigin(offset);
		//Rotation Calculation
			float angle = ((2*PI)/itemCount)*i;
			btMatrix3x3* rotationMat = new btMatrix3x3(cos(angle),0,sin(angle),
													       0     ,1,    0     ,
													  -sin(angle),0,cos(angle));

			trans.setOrigin(((*rotationMat)*(offset - m_playerPosition)) + m_playerPosition);
			trans.setRotation(btQuaternion(btVector3(0.f,1.f,0.f),angle));

		currentCShapes.push_back(m_letterDrawer.getLetters(*it));

		// Create the Rigid Body
		btRigidBody* add;
		if(*it == "THE END" || ((*it != "BACK") && (m_nextMenu == HIGHSCORE_WATCH_MENU))){
			add = m_ownerWorld->addRigidBody(MENUPOINT_MASS,trans,currentCShapes.back(),0, 0, btVector3(0,0.6f,0.2f));
		} else {
			add = m_ownerWorld->addRigidBody(MENUPOINT_MASS,trans,currentCShapes.back(),COL_LETTERS, LETTERS_COL_WITH, btVector3(0,0,0.2f));
		}
		add->setGravity(btVector3(0,0,0));
		add->setActivationState(DISABLE_DEACTIVATION);
		currentBodies.push_back(add);
		i++;

	}
	m_frontBody = currentBodies.front();
	delete m_frontPosition;
	m_frontPosition = new btVector3(currentBodies.front()->getWorldTransform().getOrigin().getX(),
									currentBodies.front()->getWorldTransform().getOrigin().getY(),
									currentBodies.front()->getWorldTransform().getOrigin().getZ());
	setFront();

	createNavigationArrows();

	if(m_nextMenu == HIGHSCORE_WATCH_MENU){
		Message sendmsg;
		sendmsg.messageType = Message::MSG_HIGHSCORE_DIFF_SELECTED;
		sendmsg.stringInfo = ((LetterCompoundShape*)(this->m_frontBody->getCollisionShape()))->getType();
		sendMessage(sendmsg);
	}

//	m_frontDistance = itemCount*8 - 15;
//	setNextBody();

}

void MenuList::setFront(){
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(0,20,-12) + m_playerPosition);
	m_frontBody->setWorldTransform(trans);
}

void MenuList::setBackFront(){
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(*m_frontPosition);
	m_frontBody->setWorldTransform(trans);
}

void MenuList::createHighscoreName(){
	if(m_highscoreBody){m_ownerWorld->deleteRigidBody(m_highscoreBody);}
	if(m_highscoreShape){delete m_highscoreShape;}
	m_highscoreShape = m_letterDrawer.getLetters(m_highscoreName);
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(0,35,-10));

	m_highscoreBody = m_ownerWorld->addRigidBody(MENUPOINT_MASS,trans,m_highscoreShape,0, 0, btVector3(0.8f,0.1f,0.2f));
	m_highscoreBody->setGravity(btVector3(0,0,0));
	m_highscoreBody->setActivationState(DISABLE_DEACTIVATION);
}

void MenuList::setNextMenu(menuTypes type){
	m_nextMenu = type;
}

void MenuList::invokeMenu(menuTypes type){
	setMenuChangeCalled(true);
	setNextMenu(type);
}

int MenuList::getCurrentState(){
	return currentState;
}

void MenuList::setPlayerPosition(btVector3* playerPosition){
	m_playerPosition = *playerPosition;
}

btScalar MenuList::get_time_delta_in_ms(btScalar start){
	btScalar this_time = m_clock.getTimeMilliseconds();
	btScalar time_difference = this_time - start;
	return time_difference;
}

void MenuList::closeMenu(){
	clearMenuPoints();
	m_rotationCalled = false;
	m_menuChangeCalled = false;
}
