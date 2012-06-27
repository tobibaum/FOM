/*
 * GameConfiguration.cpp
 *
 *  Created on: May 17, 2011
 *      Author: Tobi
 */

#include "ArcadeConfiguration.h"

using namespace std;

ArcadeConfiguration::ArcadeConfiguration() {
	// TODO Auto-generated constructor stub

}

ArcadeConfiguration::~ArcadeConfiguration() {
	// TODO Auto-generated destructor stub
}

btVector3 ArcadeConfiguration::getCanonPosition(int level)
{
	list<ArcadeLevel*>::iterator levelIt;
	for(levelIt = m_levelList.begin(); levelIt != m_levelList.end(); levelIt++){
		if((*levelIt)->getLevelNumber() == level){
			return (*levelIt)->getCanonPosition();
		}
	}
	return btVector3(0,0,0);
}

btScalar ArcadeConfiguration::getShootFrequency(int level)
{
//    return m_shootFrequency;
	list<ArcadeLevel*>::iterator levelIt;
	for(levelIt = m_levelList.begin(); levelIt != m_levelList.end(); levelIt++){
		if((*levelIt)->getLevelNumber() == level){
			return (*levelIt)->getShootFrequency();
		}
	}
	return 0;
}

btScalar ArcadeConfiguration::getShootSpeed(int level)
{
//    return m_shootSpeed;
	list<ArcadeLevel*>::iterator levelIt;
	for(levelIt = m_levelList.begin(); levelIt != m_levelList.end(); levelIt++){
		if((*levelIt)->getLevelNumber() == level){
			return (*levelIt)->getShootSpeed();
		}
	}
	return 0;
}

btVector3 ArcadeConfiguration::getShootingTarget(int level)
{
	list<ArcadeLevel*>::iterator levelIt;
	for(levelIt = m_levelList.begin(); levelIt != m_levelList.end(); levelIt++){
		if((*levelIt)->getLevelNumber() == level){
			return (*levelIt)->getShootingTarget();
		}
	}
	return btVector3(0,0,0);
}

ArcadeLevel* ArcadeConfiguration::getLevel(int level){
	ArcadeLevel* result;
	list<ArcadeLevel*>::iterator levelIt;
	for(levelIt = m_levelList.begin(); levelIt != m_levelList.end(); levelIt++){
		if((*levelIt)->getLevelNumber() == level){
			result = *levelIt;
			break;
		}
	}

	return result;
}

std::list<ArcadeLevel*> ArcadeConfiguration::getLevelList()
{
    return m_levelList;
}

void ArcadeConfiguration::setLevelList(std::list<ArcadeLevel*> levelList)
{
    this->m_levelList = levelList;
}

ShapeProbability* ArcadeConfiguration::getShapeByValue(float random, int level){
	list<ArcadeLevel*>::iterator levelIt;
	ArcadeLevel* selectedLevel;
	list<ShapeProbability*> shapeProb;
	// Search for Level & ShapeList
	for(levelIt = m_levelList.begin(); levelIt != m_levelList.end(); levelIt++){
		if((*levelIt)->getLevelNumber() == level){
			selectedLevel = (ArcadeLevel*)*levelIt;
			shapeProb = ((ArcadeLevel*)*levelIt)->getShapes();
		}
	}

	//Evaluate selected Shape
	list<ShapeProbability*>::iterator shapeIt;
	float probStack=0;
	for(shapeIt = shapeProb.begin(); shapeIt != shapeProb.end(); shapeIt++){
		probStack += (*shapeIt)->getProbability();
		if (random <= probStack){
			return *shapeIt;
		}
	}
	printf("i shouldnt reach this point\n");
	return 0;
}


