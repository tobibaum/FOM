/*
 * MenuList.h
 *
 *  Created on: May 17, 2011
 *      Author: hendrik
 */

#ifndef MENULIST_H_
#define MENULIST_H_

#define MENUPOINT_MASS 5
#define MENU_TRIGGER_RANGE 4

#include "../ColoredWorld.h"
#include <list>
#include <iostream>
#include "LetterDrawer.h"
#include "../CollisionClasses.h"
#include "SignalUser.h"
#include "../Skeleton.h"
#include "../GameConfigurations/ArcadeConfiguration.h"

class MenuList : public SignalUser{
public:
	MenuList(ColoredWorld* ownerWorld);
	virtual ~MenuList();

	enum highScoreCallTypes{
		HS_CALL_ARCADE = 0,
		HS_CALL_SPARTA
	};

	enum menuTypes{
		MAIN_MENU = 0,
		HIGHSCORE_MENU,
		HIGHSCORE_WATCH_MENU,
		HIGHSCORE_SELECT_MENU,
		STATISTICS_MENU,
		MODE_MENU,
		PAUSE_MENU,
		GO_MENU,
		INSTRUCTIONS_MENU,
		DIFFICULTY_MENU
	};

	enum selectionType{
		SEL_PLAY_GAME = 0,
		SEL_HIGHSCORES,
		SEL_EXIT_CALLED,
		SEL_ARCADE,
		SEL_SPARTA,
		SEL_NEXT_LEVEL,
		SEL_HIT_TRAINING,
		SEL_EASY,
		SEL_MEDIUM,
		SEL_HARD,
		SEL_MADNESS,
		SEL_BACK,
		SEL_ROTATE_LEFT,
		SEL_ROTATE_RIGHT,
		SEL_SAVE_HS,
		SEL_CORRECT,
		SEL_END,
		SEL_GO,
		SEL_WON,
		SEL_SINGLE_LETTER,
		SEL_HIGHSCORE_ENTERED,
		SEL_CHANGE_PLAYER,
		SEL_INSTRUCTIONS,
		SEL_RETURN
	};

	enum directionTypes{
		ROT_LEFT = 0,
		ROT_RIGHT
	};

private:
    ColoredWorld *m_ownerWorld;
    LetterDrawer m_letterDrawer;
    int currentState;
    bool m_rotationCalled;
    bool m_menuChangeCalled;
    bool m_arrowsDeleted;
public:
    void trigger(Message msg);
    void closeMenu();
    void createMainMenu();
    void createGoMenu();
    void createInstructionsMenu();
    void createHighscoreMenu();
    void createHighscoreWatchMenu();
    void createHighscoreSelectMenu();
    void createPlayGameMenu();
    void createDifficultyMenu();
    void createWonMenu();
    void createStatisticsMenu();
    void createPauseMenu();
    void createEndMenu();
    void setPlayerPosition(btVector3 *playerPosition);
    void process(float ms);

    bool getRotationCalled() const;
    void setRotationCalled(bool rotationCalled);

    void clearMenuPoints();
    bool getMenuChangeCalled() const;
    void setMenuChangeCalled(bool m_menuChangeCalled);
    void invokeMenu(menuTypes type);
private:
    void setNextMenu(menuTypes type);
    void createMenuPoints();
    void createNextMenu();
    void createHighscoreName();
    void initMap();
    int getCurrentState();
//    void moveInOut(btScalar step);
//    void moveInOutUndo();
//    void setNextBody();
    void rotate(float ms);
    void createNavigationArrows();
    void deleteNavigationArrows();
    void setFront();
    void setBackFront();
    btCompoundShape *m_Rnav_shape;
    btCompoundShape *m_Lnav_shape;
    btRigidBody *m_Rnav_body;
    btRigidBody *m_Lnav_body;

    btRigidBody* m_frontBody;
    btRigidBody* m_nextBody;
    btVector3* m_frontPosition; // not needed anymore
    btScalar m_frontDistance;
    btScalar m_absDistance;


    std::list<std::string> actualMenuPoints;
    std::list<btCompoundShape*> currentCShapes;
    std::list<btRigidBody*> currentBodies;
    std::map<std::string,selectionType> menuMap;
    btVector3 m_playerPosition;
    btScalar get_time_delta_in_ms(btScalar start);
    btScalar m_time_for_one_rotation;


    int m_rotation_direction;
	btScalar m_passed_in_rotation;
	btClock m_clock;
	btScalar m_last_time;

	btScalar m_bufferTime;
	btScalar m_bufferTimeThreshold;

	int m_nextMenu;

	Skeleton*  m_skeleton;

	//Highscore Stuff

	std::list<btCollisionShape*> highscoreShapeList;
	std::list<btRigidBody*> highscoreBodyList;

	std::string m_highscoreName;
	btCollisionShape* m_highscoreShape;
	btRigidBody* m_highscoreBody;

	bool m_arcade_selected;

	bool m_highscore_selected;

	int m_currentDiff;


};

#endif /* MENULIST_H_ */
