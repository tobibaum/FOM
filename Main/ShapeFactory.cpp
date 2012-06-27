/*
 * ShapeFactory.cpp
 *
 *  Created on: Jun 18, 2011
 *      Author: hendrik
 */

#include "ShapeFactory.h"
#include "../src/BulletCollision/Gimpact/btGImpactShape.h"
#include "LooseCompoundShape.h"
#include "CakeShape.h"

using namespace std;

#define PI 3.1415926535897932384626433832795028841971

ShapeFactory::ShapeFactory() {
	// TODO Auto-generated constructor stub

}

ShapeFactory::~ShapeFactory() {
	// TODO Auto-generated destructor stub
}

btCollisionShape* ShapeFactory::getShape(shapeTypes type){
	btCollisionShape* colShape;
	switch(type){

	case BULLET_SHAPE 		: return new btCapsuleShape(2.f,1.f);
	case CANNONBALL_SHAPE 	: return new btSphereShape(2.5);
	case KANTHOLZ_SHAPE		: return new btBoxShape(btVector3(0.5,0.5,4));
	case WOOD_SHAPE			: return new btCylinderShape(btVector3(2,5,0));
	case PRESENT_SHAPE		: return new btBoxShape(btVector3(1,1,1));
	case CHAIR_SHAPE		:  colShape = chairShape(); return colShape;
	case SPIKEDBALL_SHAPE	:  colShape = spikedBallShape(); return colShape;
	case CAKE_SHAPE			: colShape = cakeShape(); return colShape;
	default					: return 0;
	}
}

btCollisionShape* ShapeFactory::chairShape(){
	btCompoundShape* shape = new btCompoundShape();

	btCollisionShape* chair[6];
	btTransform trans;
	trans.setIdentity();

	chair[0] = new btCapsuleShape(0.5,2);
	trans.setOrigin(btVector3(-1.1,1,1.1));
	shape->addChildShape(trans,chair[0]);

	chair[1] = new btCapsuleShape(0.5,2);
	trans.setOrigin(btVector3(1.1,1,1.1));
	shape->addChildShape(trans,chair[1]);

	chair[2] = new btCapsuleShape(0.5,5);
	trans.setOrigin(btVector3(-1.1,2.5,-2.2));
	shape->addChildShape(trans,chair[2]);

	chair[3] = new btCapsuleShape(0.5,5);
	trans.setOrigin(btVector3(1.1,2.5,-2.2));
	shape->addChildShape(trans,chair[3]);

	chair[4] = new btBoxShape(btVector3(1.5,0.25,1.7));
	trans.setOrigin(btVector3(0,2,-0.5));
	shape->addChildShape(trans,chair[4]);

	chair[5] = new btCapsuleShape(0.5,2.3);
	trans.setOrigin(btVector3(0,5,-2.2));
	trans.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	shape->addChildShape(trans,chair[5]);

	return shape;
}

btQuaternion* ShapeFactory::calculateRotation(btVector3* axis, btVector3* targetAxis) {
	//printf("axis: %f x %f x %f \n", axis->getX(),axis->getY(),axis->getZ());
	//printf("targetAxis: %f x %f x %f \n", targetAxis->getX(),targetAxis->getY(),targetAxis->getZ());
	btVector3 cross = targetAxis->cross(*axis);
	btScalar rotation_angle = targetAxis->angle(*axis);
	return new btQuaternion(cross, -rotation_angle);
}

btCollisionShape* ShapeFactory::spikedBallShape(){
	btCompoundShape* shape = new btCompoundShape();

	btCollisionShape* spikes;
	btTransform trans;
	trans.setIdentity();

	trans.setOrigin(btVector3(0,0,0));
	shape->addChildShape(trans,new btSphereShape(2));

	spikes = new btConeShape(0.7,1.2);

	trans.setOrigin(btVector3(0,2,0));
	shape->addChildShape(trans,spikes);

	trans.setOrigin(btVector3(2,0,0));
	trans.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(1,0,0)));
	shape->addChildShape(trans,spikes);

	trans.setOrigin(btVector3(-2,0,0));
	trans.setRotation(*calculateRotation(new btVector3(0,1,0),new btVector3(-1,0,0)));
	shape->addChildShape(trans,spikes);

	trans.setOrigin(btVector3(0,-2,0));
	trans.setRotation(btQuaternion(0,0,PI));
	shape->addChildShape(trans,spikes);

	trans.setOrigin(btVector3(0,0,2));
	trans.setRotation(btQuaternion(0,PI/2,0));
	shape->addChildShape(trans,spikes);

	trans.setOrigin(btVector3(0,0,-2));
	trans.setRotation(btQuaternion(0,PI*1.5,0));
	shape->addChildShape(trans,spikes);

	return shape;
}

btCollisionShape* ShapeFactory::cakeShape(){
	return createCakeShape(btVector3(0,0,0),
							btVector3(5,0,-5),
							btVector3(0,0,-5),
							btVector3(2.5,5,-5),
							btVector3(5,0,0),
							btVector3(2.5,5,0));
}

btCompoundShape* ShapeFactory::getCakeWithHeight(btVector3 v1, btVector3 v2, btVector3 v3, btScalar height){
	btVector3 v4 = v1 + btVector3(0,0,height);
	btVector3 v5 = v2 + btVector3(0,0,height);
	btVector3 v6 = v3 + btVector3(0,0,height);

	return createCakeShape(v1,v5,v4,v6,v2,v3);

}

btCompoundShape* ShapeFactory::createCakeShape(btVector3 v1, btVector3 v2, btVector3 v3, btVector3 v4, btVector3 v5, btVector3 v6){
	CakeShape* shape = new CakeShape();

	btTetrahedronShapeEx* cake1;
	btTetrahedronShapeEx* cake2;
	btTetrahedronShapeEx* cake3;

	btTransform trans;
	trans.setIdentity();

	cake1 = new btTetrahedronShapeEx();
	cake2 = new btTetrahedronShapeEx();
	cake3 = new btTetrahedronShapeEx();
	cake1->setVertices(v1,v2,v3,v4);
	cake2->setVertices(v1,v2,v5,v4);
	cake3->setVertices(v1,v5,v4,v6);

	shape->addChildShape(trans,cake1);
	shape->addChildShape(trans,cake2);
//	shape->addChildShape(trans,cake3);

	return shape;
}

btCollisionShape* ShapeFactory::getTriangleWall(std::list<triangle> triangleList, btScalar height){
	LooseCompoundShape* triangleWall = new LooseCompoundShape();
	btTransform trans;
	trans.setIdentity();

	for(list<triangle>::iterator tri_it = triangleList.begin(); tri_it!= triangleList.end(); tri_it++ ){
		CakeShape* cakePiece = (CakeShape*)getCakeWithHeight((btVector3)tri_it->p1, (btVector3)tri_it->p2, (btVector3)tri_it->p3, height);
		cakePiece->setTri(*tri_it);
		triangleWall->addChildShape(trans,cakePiece);
	}

	return triangleWall;
}


