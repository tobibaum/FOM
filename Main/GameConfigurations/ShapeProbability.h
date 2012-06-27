/*
 * ShapeProbability.h
 *
 *  Created on: Jun 28, 2011
 *      Author: hendrik
 */

#ifndef SHAPEPROBABILITY_H_
#define SHAPEPROBABILITY_H_

#include "btBulletDynamicsCommon.h"

class ShapeProbability {
public:
	ShapeProbability();
	virtual ~ShapeProbability();
    float getProbability() const;
    btCollisionShape *getShape() const;
    void setProbability(float probability);
    void setShape(btCollisionShape *shape);
    float getProbBad() const;
    float getProbGood() const;
    float getProbReflect() const;
    void setProbBad(float probBad);
    void setProbGood(float probGood);
    void setProbReflect(float probReflect);

private:
	btCollisionShape* shape;
	float probability;
	float probGood;
	float probBad;
	float probReflect;
};

#endif /* SHAPEPROBABILITY_H_ */
