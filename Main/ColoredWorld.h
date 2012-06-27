/*
 * ColoredWorld.h
 *
 *  Created on: May 11, 2011
 *      Author: Tobi
 */

#ifndef COLOREDWORLD_H_
#define COLOREDWORLD_H_

#include "btBulletDynamicsCommon.h"
#include "Object.h"
#include "CollisionClasses.h"
#include "FlyingObject.h"
#include "WallObject.h"
#include "CakeObject.h"
#include <list>
#include <map>
#include <string>
#include <utility>
#include "ShapeFactory.h"
#include "LooseCompoundShape.h"


class Object;

/**
 * This class will serve to decorate the dynamic Objects with color/...
 * Its responsibility is to update the dynamicsWorld and the stored object and
 * to keep them consistent.
 */

struct frozenObject{
	btRigidBody* body;
	btVector3 linVel;
	btVector3 angVel;
	btVector3 gravity;
};

class ColoredWorld : public SignalUser{
	friend class DemoApplication;
	friend class CollisionDetector;
	friend class Field;
protected:
	ColoredWorld(btDispatcher* dispatcher,btBroadphaseInterface* pairCache,
			btConstraintSolver* constraintSolver, btCollisionConfiguration* collisionConfiguration);
	virtual ~ColoredWorld();

public:
	static ColoredWorld* getInstance();
	static void destroy();

    btRigidBody *addRigidBody(btScalar mass, const btTransform & startTransform, btCollisionShape *shape, short int cType, short int cMask, btVector3 color = btVector3(.5f, .5f, .5f), btScalar transparency = 0.f);
    btRigidBody *addFlyingObject(btScalar mass, const btTransform & startTransform, btCollisionShape *shape, FlyingObject::FlyingObjectType type);
    LooseCompoundShape *addWallObject(const btTransform & startTransform, std::string filename, float wallDepth, float wallHeight);
    LooseCompoundShape *addTriangles(const btTransform & startTransform,std::list<triangle> tris, float wallDepth, float linVel, CakeObject *object);
    void deleteRigidBody(btRigidBody* rigidBody);
    int	stepSimulation( btScalar timeStep,int maxSubSteps=1, btScalar fixedTimeStep=btScalar(1.)/btScalar(60.));
    int getNumCollisionObjects();
    void setGravity(btVector3 vector);
    void cleanup();
    void addTexture(btRigidBody* body, std::string filename);

    void trigger(Message msg);

    void freeze();
    void unfreeze();

    std::list<Object*> getObjects();
    std::list<Object*> getLetterObjects();

    Object* getObject(btRigidBody* rigidBody);
    Object* getObject(btCollisionObject* object);

private: /* functions */

    btDynamicsWorld* getDynamicsWorld();


private: /* member */
    btDynamicsWorld* m_dynamicsWorld;
	std::map<btRigidBody*,Object*> m_objects;
	std::map<btRigidBody*,Object*> m_transparentobjects;

	std::list<Object*> m_letterObjects;
	std::list< std::pair<btClock*, btRigidBody*> > m_stashedForDeletion;

	std::map<btCollisionObject*, Object*> m_collMap;

	std::list<frozenObject> m_frozenObjects;
};

#endif /* COLOREDWORLD_H_ */
