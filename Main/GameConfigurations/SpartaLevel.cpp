/*
 * SpartaLevel.cpp
 *
 *  Created on: Jul 4, 2011
 *      Author: hendrik
 */

#include "SpartaLevel.h"

SpartaLevel::SpartaLevel() {
	// TODO Auto-generated constructor stub

}

SpartaLevel::~SpartaLevel() {
	// TODO Auto-generated destructor stub
}

float SpartaLevel::getFrequency() const
{
    return frequency;
}

std::list<WallProbability*> SpartaLevel::getLevelList() const
{
    return levelList;
}

int SpartaLevel::getLevelNumber() const
{
    return levelNumber;
}

float SpartaLevel::getSpeed() const
{
    return speed;
}

btVector3 *SpartaLevel::getStartPosition() const
{
    return startPosition;
}

void SpartaLevel::setFrequency(float frequency)
{
    this->frequency = frequency;
}

void SpartaLevel::setLevelList(std::list<WallProbability*> levelList)
{
    this->levelList = levelList;
}

void SpartaLevel::setLevelNumber(int levelNumber)
{
    this->levelNumber = levelNumber;
}

void SpartaLevel::setSpeed(float speed)
{
    this->speed = speed;
}

void SpartaLevel::setStartPosition(btVector3 *startPosition)
{
    this->startPosition = startPosition;
}

int SpartaLevel::getBallThresh() const
{
    return ballThresh;
}

void SpartaLevel::setBallThresh(int ballThresh)
{
    this->ballThresh = ballThresh;
}




