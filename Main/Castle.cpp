/*
 * Castle.cpp
 *
 *  Created on: May 4, 2011
 *      Author: Tobi
 */

#include "Castle.h"
#include "CastlePointCounter.h"

using namespace std;

///scaling of the objects (0.1 = 20 centimeter boxes )
#define SCALING 1.

///create 125 (5x5x5) dynamic object
#define ARRAY_SIZE_X 5
#define ARRAY_SIZE_Y 5
#define ARRAY_SIZE_Z 1


Castle::Castle(int in_x, int in_z) {
	m_coloredWorld = ColoredWorld::getInstance();

	m_basicValue = 0;

	create(in_x, in_z);

}

Castle::~Castle() {
	for(list<CastleObject*>::iterator part_it = m_parts.begin();
			part_it != m_parts.end(); part_it++){
		(*part_it)->deleteRigid();
	}

	CastlePointCounter::getInstance()->deleteCastle(this);

	m_parts.clear();
}

float Castle::getPoints(){
	float result = 0;

	if(m_parts.size() != 0){
		for(list<CastleObject*>::iterator castle_it = m_parts.begin();
				castle_it != m_parts.end(); castle_it++){
			result += (*castle_it)->getPoints();
		}
	}

	doRemoval();

	return result + m_basicValue;
}

void  Castle::create(int in_x, int in_z) {
	//create a few dynamic rigidbodies
	// Re-using the same collision is better for memory usage and performance

	btCollisionShape* colShape = new btBoxShape(btVector3(SCALING*1,SCALING*2,SCALING*.5));
	//btCollisionShape* colShape = new btSphereShape(btScalar(1.));

	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar	mass(1.f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass,localInertia);

	float start_x = in_x - ARRAY_SIZE_X/2;
	float start_y = 2;
	float start_z = in_z - ARRAY_SIZE_Z/2;

	int color_count = 0;
	btVector3 color;

	for (int k=0;k<ARRAY_SIZE_Y;k++)
	{
		for (int i=0;i<ARRAY_SIZE_X;i++)
		{
			for(int j = 0;j<ARRAY_SIZE_Z;j++)
			{
				startTransform.setOrigin(SCALING*btVector3(
									btScalar(2.0*i + start_x),
									btScalar(2*2.0*k + start_y),
									btScalar(.5*2.0*j + start_z)));


//				//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
//				btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
//				btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
//				btRigidBody* body = new btRigidBody(rbInfo);

				if(color_count == 0){
					color = btVector3(1.f,1.f,1.f);
					color_count = 1;
				} else {
					color = btVector3(.9f,0.9f,.9f);
					color_count = 0;
				}

				btRigidBody* castle = m_coloredWorld->addRigidBody(mass,startTransform,colShape,COL_CASTLE,CASTLE_COL_WITH,color);
				m_coloredWorld->addTexture(castle,"wood.jpg");
				CastleObject* thisObject = (CastleObject*)m_coloredWorld->getObject(castle);
				thisObject->setFatherCastle(this);

				m_parts.push_back(thisObject);

			}
		}
	}



	CastlePointCounter::getInstance()->addCastle(this);
}

void Castle::doRemoval(){
	for(list<CastleObject*>::iterator castle_it = m_stashedForErase.begin();
			castle_it != m_stashedForErase.end(); castle_it++){
		m_parts.remove(*castle_it);
	}

	m_stashedForErase.clear();
}


void Castle::removePart(CastleObject* part, float points){
	m_basicValue += points;
	m_stashedForErase.push_back(part);
}

btScalar Castle::getPointFactor() const
{
    return m_pointFactor;
}

void Castle::setPointFactor(btScalar m_pointFactor)
{
	for(list<CastleObject*>::iterator castle_it = m_parts.begin();
			castle_it != m_parts.end(); castle_it++){
		(*castle_it)->setPointFactor(m_pointFactor);
	}
    this->m_pointFactor = m_pointFactor;
}


