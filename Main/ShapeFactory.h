/*
 * ShapeFactory.h
 *
 *  Created on: Jun 18, 2011
 *      Author: hendrik
 */

#ifndef SHAPEFACTORY_H_
#define SHAPEFACTORY_H_

#include "btBulletDynamicsCommon.h"
#include <list>
#include "Triangle.h"

class CakeShape;


class ShapeFactory {
public:
	ShapeFactory();
	virtual ~ShapeFactory();

	enum shapeTypes{
		BULLET_SHAPE = 0,
		CANNONBALL_SHAPE,
		KANTHOLZ_SHAPE,
		WOOD_SHAPE,
		PRESENT_SHAPE,
		CHAIR_SHAPE,
		SPIKEDBALL_SHAPE,
		CAKE_SHAPE
	};

public:
	btCollisionShape* getShape(shapeTypes type);
	btCompoundShape* getCakeWithHeight(btVector3 v1, btVector3 v2, btVector3 v3, btScalar height);
	btCollisionShape* getTriangleWall(std::list<triangle> triangleList, btScalar height);

private:
	/*Complex Shape methods*/
	btCollisionShape* chairShape();
	btCollisionShape* spikedBallShape();
	btCollisionShape* cakeShape();
	btCompoundShape* createCakeShape(btVector3 v1, btVector3 v2, btVector3 v3, btVector3 v4, btVector3 v5, btVector3 v6);

	btQuaternion* calculateRotation(btVector3* axis, btVector3* targetAxis);

};

#endif /* SHAPEFACTORY_H_ */
