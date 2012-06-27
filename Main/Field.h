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
#ifndef BASIC_DEMO_H
#define BASIC_DEMO_H

#ifdef _WINDOWS
#include "Win32DemoApplication.h"
#define PlatformDemoApplication Win32DemoApplication
#else
#include "GlutDemoApplication.h"
#define PlatformDemoApplication GlutDemoApplication
#endif

#include "LinearMath/btAlignedObjectArray.h"
#include "Cannon.h"
#include "SpartaCannon.h"
#include "Player.h"
#include "Castle.h"
#include "CollisionDetector.h"
#include "PointingSystem.h"
#include "Camera.h"
#include "Cleaner.h"
#include "SignalSlot/SignalUser.h"
#include "GameConfigurations/ArcadeConfiguration.h"
#include "Menu/MenuList.h"
#include "HandTracker.h"
//#include "SpeechSynch.h"

#include "GameConfigurations/ConfigLoader.h"
#include "HighscoreHandler.h"
#include "SoundManager.h"
#include "LevelHandler.h"
#include "CastlePointCounter.h"
#include "Colorer.h"

class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;
class Camera;
class SignalUser;

///Field is good starting point for learning the code base and porting.

class Field : public PlatformDemoApplication
{
public:

	Field();
	virtual ~Field()
	{
		exitPhysics();
	}
	bool	initPhysics();

	virtual void initWorld() = 0;

	virtual void renderOverlay(){

	}

	void	exitPhysics();

	virtual void clientMoveAndDisplay();

	virtual void displayCallback();
	virtual void	clientResetScene();
	

protected:
	virtual void step() = 0;

protected: /* functions */
	void shootMyBox(const btVector3& destination);

	void createStadion();

private:

	void trigger(Message msg);

	void startGame(ArcadeConfiguration::difficulties difficulty, ArcadeConfiguration::gameMode mode);

	void renderString(std::string output, int x, int y);

	void drawRectangleOnScreen(int x1, int y1, int x2, int y2);

	void drawSpot();

	void renderSkyBox();

	void renderSkyPlane(int m11, int m12,int m13,
			int m21,int m22,int m23,
			int m31,int m32,int m33,
			int m41,int m42,int m43);

protected: /* members */
	//keep the collision shapes, for deletion/cleanup
	btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;

	btBroadphaseInterface*	m_broadphase;

	btCollisionDispatcher*	m_dispatcher;

	btConstraintSolver*	m_solver;

	btDefaultCollisionConfiguration* m_collisionConfiguration;

	Cannon* m_canon;

	SpartaCannon* m_spartaCannon;

	Skeleton* m_skel;

	Player* m_player;

	Castle* m_castle;
	
	CollisionDetector* m_collDect;

	PointingSystem* m_pointing;

	CastlePointCounter* m_castlePointCount;

	Camera* m_camera;

	btVector3 m_camPos;

	Cleaner* m_cleaner;

	Colorer* m_colorer;

	HandTracker* m_tracker;

	MenuList* m_menu;

	ConfigLoader m_configLoader;

	HighscoreHandler m_highscoreHandler;

	LevelHandler* m_levelHandler;

	std::list<PlayerPoints> m_tempHighscoreList;

	SoundManager* m_sounds;

	bool m_highscoreCalled;


//	SpeechSynch* m_speech;

	bool m_pause;

	bool m_ended;

	bool m_gameStarted;

	bool m_showExplanation;

	bool m_showCalibrateHelpMessage;

	bool m_startFlight;

	bool m_showStatistics;

	bool m_showSpotOnGround;

	int m_highscoreTypeRequested;
	std::string m_highscoreDiffState;

	bool m_showFOM;
	bool m_bFlightdone;

	bool m_instructionMenu;

	float m_fShowFOMTime;

	int m_currentMode;

	int m_currentDifficulty;

	std::list<Castle*> castleList;

};

#endif //BASIC_DEMO_H

