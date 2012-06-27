/*
 * Level.cpp
 *
 *  Created on: Jun 28, 2011
 *      Author: hendrik
 */

#include "ArcadeLevel.h"

ArcadeLevel::ArcadeLevel() {
	// TODO Auto-generated constructor stub

}

ArcadeLevel::~ArcadeLevel() {
	// TODO Auto-generated destructor stub
}

btVector3 ArcadeLevel::getCanonPosition() const
{
    return canonPosition;
}

int ArcadeLevel::getLevelNumber() const
{
    return levelNumber;
}

std::list<ShapeProbability*> ArcadeLevel::getShapes() const
{
    return shapes;
}

btScalar ArcadeLevel::getShootFrequency() const
{
    return shootFrequency;
}

btScalar ArcadeLevel::getShootSpeed() const
{
    return shootSpeed;
}

btVector3 ArcadeLevel::getShootingTarget() const
{
    return shootingTarget;
}

std::string ArcadeLevel::getWinConditionFLAG() const
{
    return winConditionFLAG;
}

void ArcadeLevel::setCanonPosition(btVector3 canonPosition)
{
    this->canonPosition = canonPosition;
}

void ArcadeLevel::setLevelNumber(int levelNumber)
{
    this->levelNumber = levelNumber;
}

void ArcadeLevel::setShapes(std::list<ShapeProbability*> shapes)
{
    this->shapes = shapes;
}

void ArcadeLevel::setShootFrequency(btScalar shootFrequency)
{
    this->shootFrequency = shootFrequency;
}

void ArcadeLevel::setShootSpeed(btScalar shootSpeed)
{
    this->shootSpeed = shootSpeed;
}

void ArcadeLevel::setShootingTarget(btVector3 shootingTarget)
{
    this->shootingTarget = shootingTarget;
}

void ArcadeLevel::setWinConditionFLAG(std::string winConditionFLAG)
{
    this->winConditionFLAG = winConditionFLAG;
}

int ArcadeLevel::getBallThresh() const
{
    return ballThresh;
}

int ArcadeLevel::getLiveThresh() const
{
    return liveThresh;
}

int ArcadeLevel::getPointThresh() const
{
    return pointThresh;
}

int ArcadeLevel::getTimeThresh() const
{
    return timeThresh;
}

void ArcadeLevel::setBallThresh(int ballThresh)
{
    this->ballThresh = ballThresh;
}

void ArcadeLevel::setLiveThresh(int liveThresh)
{
    this->liveThresh = liveThresh;
}

void ArcadeLevel::setPointThresh(int pointThresh)
{
    this->pointThresh = pointThresh;
}

void ArcadeLevel::setTimeThresh(int timeThresh)
{
    this->timeThresh = timeThresh;
}




