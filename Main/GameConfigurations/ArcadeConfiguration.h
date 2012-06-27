/*
 * GameConfiguration.h
 *
 *  Created on: May 17, 2011
 *      Author: Tobi
 */


#ifndef ARCADECONFIGURATION_H_
#define ARCADECONFIGURATION_H_

#include <list>
#include <iostream>
#include "btBulletDynamicsCommon.h"
#include "ArcadeLevel.h"

class ArcadeConfiguration {
public:
	ArcadeConfiguration();
	virtual ~ArcadeConfiguration();


	enum gameMode{
		MODE_ARCADE,
		MODE_SPARTA
	};

	enum difficulties{
		DIFF_EASY = 0,
		DIFF_MEDIUM,
		DIFF_HARD,
		DIFF_MADNESS,
		DIFF_TRAINING,
		DIFF_NUM_DIFFICULTIES
	};


//	virtual void breakCondition() = 0;

	ArcadeLevel* getLevel(int level);
    btVector3 getCanonPosition(int level);
    btScalar getShootFrequency(int level);
    btScalar getShootSpeed(int level);
    btVector3 getShootingTarget(int level);
    void setShootSpeed(btScalar m_shootSpeed);
    std::list<ArcadeLevel*> getLevelList();
    void setLevelList(std::list<ArcadeLevel*> levelList);

    ShapeProbability* getShapeByValue(float random, int level);

private:

	std::list<ArcadeLevel*> m_levelList;
	int m_numberOfLevels;
};

#endif /* GAMECONFIGURATION_H_ */
