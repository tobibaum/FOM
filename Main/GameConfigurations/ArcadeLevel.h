/*
 * Level.h
 *
 *  Created on: Jun 28, 2011
 *      Author: hendrik
 */

#ifndef ARCADELEVEL_H_
#define ARCADELEVEL_H_

#include "btBulletDynamicsCommon.h"
#include "ShapeProbability.h"
#include <list>
#include <iostream>

class ArcadeLevel {
public:
	ArcadeLevel();
	virtual ~ArcadeLevel();
    btVector3 getCanonPosition() const;
    int getLevelNumber() const;
    std::list<ShapeProbability*> getShapes() const;
    btScalar getShootFrequency() const;
    btScalar getShootSpeed() const;
    btVector3 getShootingTarget() const;
    std::string getWinConditionFLAG() const;

    void setCanonPosition(btVector3 canonPosition);
    void setLevelNumber(int levelNumber);
    void setShapes(std::list<ShapeProbability*> shapes);
    void setShootFrequency(btScalar shootFrequency);
    void setShootSpeed(btScalar shootSpeed);
    void setShootingTarget(btVector3 shootingTarget);
    void setWinConditionFLAG(std::string winConditionFLAG);
    int getBallThresh() const;
    int getLiveThresh() const;
    int getPointThresh() const;
    int getTimeThresh() const;
    void setBallThresh(int ballThresh);
    void setLiveThresh(int liveThresh);
    void setPointThresh(int pointThresh);
    void setTimeThresh(int timeThresh);
private:
    btVector3 canonPosition;
	btVector3 shootingTarget;
	btScalar shootFrequency;
	btScalar shootSpeed;

	int levelNumber;
	std::list<ShapeProbability*> shapes;
	std::string winConditionFLAG;

	int pointThresh;
	int timeThresh;
	int ballThresh;
	int liveThresh;
};

#endif /* LEVEL_H_ */
