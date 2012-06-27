/*
 * SpartaLevel.h
 *
 *  Created on: Jul 4, 2011
 *      Author: hendrik
 */

#ifndef SPARTALEVEL_H_
#define SPARTALEVEL_H_

#include "btBulletDynamicsCommon.h"
#include "WallProbability.h"
#include <list>

class SpartaLevel {
public:
	SpartaLevel();
	virtual ~SpartaLevel();
    float getFrequency() const;
    std::list<WallProbability*> getLevelList() const;
    int getLevelNumber() const;
    float getSpeed() const;
    btVector3 *getStartPosition() const;
    void setFrequency(float frequency);
    void setLevelList(std::list<WallProbability*> levelList);
    void setLevelNumber(int levelNumber);
    void setSpeed(float speed);
    void setStartPosition(btVector3 *startPosition);
    int getBallThresh() const;
    void setBallThresh(int ballThresh);

private:
	std::list<WallProbability*> levelList;
	int levelNumber;
	btVector3* startPosition;
	float frequency;
	float speed;

	int ballThresh;

};

#endif /* SPARTALEVEL_H_ */
