/*
 * ColoredWorld.cpp
 *
 *  Created on: May 11, 2011
 *      Author: Tobi
 */

#include "ColoredWorld.h"
#include "GlutStuff.h"
#include "CollisionDetector.h"
#include "LoadWall.h"
#include "CakeShape.h"
#include "CakeObject.h"
#include "SpartaPointObject.h"
#include "CastleObject.h"


ColoredWorld* m_theInstance = 0;

using namespace std;

ColoredWorld::ColoredWorld(btDispatcher* dispatcher,btBroadphaseInterface* pairCache,
		btConstraintSolver* constraintSolver, btCollisionConfiguration* collisionConfiguration) {
	m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,pairCache,constraintSolver,
			collisionConfiguration);
	m_theInstance = this;

	setTrigger(Message::MSG_WON);
	setTrigger(Message::MSG_WALL_ERASED);
	setTrigger(Message::MSG_END_GAME);
	setTrigger(Message::MSG_END_GAME_WITHOUT_HS);
}

ColoredWorld::~ColoredWorld() {
	m_objects.clear();
	delete m_dynamicsWorld;
}

void ColoredWorld::trigger(Message msg){
	switch(msg.messageType){

	// Attention!!!
	case Message::MSG_END_GAME_WITHOUT_HS:
	case Message::MSG_END_GAME:
	case Message::MSG_WON:
		// The level has ended, we want to remove all remaining flying objects

		for(map<btRigidBody*, Object*>::iterator obj_it = m_objects.begin();
				obj_it != m_objects.end(); obj_it++){
			if((*obj_it).second->getType() == COL_FLYING_OBJECTS){
				deleteRigidBody(obj_it->first);
			}
		}

		break;
	case Message::MSG_WALL_ERASED:
		// the wall is gone, delete all spartaPointOBjects.
		for(map<btRigidBody*, Object*>::iterator obj_it = m_transparentobjects.begin();
				obj_it != m_transparentobjects.end(); obj_it++){
			if((*obj_it).second->getType() == COL_SPARTA_POINTS){
				deleteRigidBody(obj_it->first);
			}
		}
		break;
	}
}

ColoredWorld* ColoredWorld::getInstance(){
	if(!m_theInstance){
		return 0;
	}
	return m_theInstance;
}

void ColoredWorld::destroy(){
	if(m_theInstance){
		delete m_theInstance;
	}
	m_theInstance = 0;
}

int ColoredWorld::getNumCollisionObjects(){
	return m_dynamicsWorld->getNumCollisionObjects();
}

int ColoredWorld::stepSimulation(btScalar timeStep,int maxSubSteps, btScalar fixedTimeStep){
	m_dynamicsWorld->stepSimulation(timeStep);
}

void ColoredWorld::setGravity(btVector3 vector){
	m_dynamicsWorld->setGravity(vector);
}

void ColoredWorld::addTexture(btRigidBody* body, std::string filename){
	m_objects.at(body)->addTexture(filename);
}

btRigidBody* ColoredWorld::addRigidBody(btScalar mass,
		const btTransform& startTransform, btCollisionShape* shape,
		short int cType, short int cMask, btVector3 color , btScalar transparency ) {

	string looseComp = "LooseCompound";

	btVector3 localInertia(0, 0, 0);

	bool isDynamic = (mass != 0.f);

	btRigidBody* body = 0;

	if (isDynamic) {

		shape->calculateLocalInertia(mass, localInertia);
		btDefaultMotionState* myMotionState = new btDefaultMotionState(
				startTransform);

		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState,
				shape, localInertia);

		body = new btRigidBody(cInfo);
		body->setContactProcessingThreshold(BT_LARGE_FLOAT);
	} else {
		body = new btRigidBody(mass, 0, shape);
		body->setWorldTransform(startTransform);
	}


	m_dynamicsWorld->addRigidBody(body, cType, cMask);

	btCollisionObject* collObj = m_dynamicsWorld->getCollisionObjectArray()
			[m_dynamicsWorld->getNumCollisionObjects() - 1];

	Object* insert_obj = new Object(color,collObj,transparency, cType, body);

	if(cType == COL_FLYING_OBJECTS){
		delete insert_obj;
		insert_obj = new FlyingObject(color,collObj,transparency, cType, body);
	} else if(cType == COL_WALL ){
		delete insert_obj;
		insert_obj = new WallObject(color,collObj,transparency, cType, body);
	} else if(cType == COL_CAKE){
		delete insert_obj;
		insert_obj = new CakeObject(color,collObj,transparency, cType, body);
	} else if(cType == COL_SPARTA_POINTS){
		delete insert_obj;
		insert_obj = new SpartaPointObject(color,collObj,transparency, cType, body);
	} else if(cType == COL_CASTLE){
		delete insert_obj;
		insert_obj = new CastleObject(color,collObj,transparency, cType, body);
	} else if(cType == COL_LETTERS){
		m_letterObjects.push_back(insert_obj);
	}

	if(transparency != 0){
		m_transparentobjects[body] = insert_obj;
	} else {
		m_objects[body] = insert_obj;
	}

	m_collMap[collObj] = insert_obj;

	return body;
}


btRigidBody *ColoredWorld::addFlyingObject(btScalar mass, const btTransform & startTransform,
		btCollisionShape *shape, FlyingObject::FlyingObjectType type){
	btVector3 color;
	switch(type){
	case FlyingObject::FOM_BAD:
		color = btVector3(1,0,0);
		break;
	case FlyingObject::FOM_GOOD:
		color = btVector3(0,1,0);
		break;
	case FlyingObject::FOM_REFLECT:
		color = btVector3(0,0,1);
		break;
	}

	btRigidBody* rigidFlying = addRigidBody(mass,startTransform,shape,COL_FLYING_OBJECTS,FLYINGOBJECTS_COL_WITH,color,0);

	Object* object = m_objects.at(rigidFlying);
	((FlyingObject*)object)->setType(type);
//	FlyingObject* flyingObject = new FlyingObject(*(FlyingObject*)object);
//
//	m_objects.erase(rigidFlying);
//	delete object;
//	m_objects[rigidFlying] = flyingObject;
//	flyingObject->setType(type);

//	m_objects.at(rigidFlying)->collided();

	return rigidFlying;
}

LooseCompoundShape *ColoredWorld::addWallObject(const btTransform & startTransform,string filename, float wallDepth, float wallHeight){
	LoadWall* loader = new LoadWall();

	list<btVector3> points = loader->getPoints(filename,wallHeight);
	list<triangle> triList = loader->getTriangles(filename,wallHeight);
	list<triangle> innerTriList = loader->getInnerTriangles(filename,wallHeight);
	float *framePoints = loader->getFrame(filename, wallHeight);

	delete loader;

//	printf("tris: %d\n", triList.size());
	ShapeFactory* fac = new ShapeFactory();
	btCollisionShape* wall = fac->getTriangleWall(triList, wallDepth);
//	printf("children: %d\n", ((LooseCompoundShape*)wall)->getNumChildShapes());
//	btCollisionShape* innerWall = fac->getTriangleWall(innerTriList, wallDepth);
	float mass = 1;

	btVector3 color = btVector3(1,1,1);
	btScalar transparency = 0;
	btRigidBody* rigidWall = addRigidBody(1,startTransform,wall,COL_WALL,WALL_COL_WITH,color, transparency);
	rigidWall->setGravity(btVector3(0,0,0));
	WallObject* object = (WallObject*)getObject(rigidWall);

	// we have a wall and thus a LooseCompoundShape.
	// This has to added in a different way, i.e., each child as itself
	LooseCompoundShape* looseWall = (LooseCompoundShape*) wall;
//	LooseCompoundShape* looseInnerWall = (LooseCompoundShape*) innerWall;


	list<btRigidBody*> rigidBodies;
	list<pair<btRigidBody*, triangle> > rigidTrianglePairs;
	map<btRigidBody*, Object*> rigidObjectMap;

	for(int i = 0; i < 1;i++){
		LooseCompoundShape* looseCompFather;
		CakeObject::CakeType cakeType;

		switch(i){
		case 0:
			looseCompFather = looseWall;
			cakeType = CakeObject::CAKE_TYPE_WALL;
			break;
		case 1:
//			looseCompFather = looseInnerWall;
			cakeType = CakeObject::CAKE_TYPE_HOLE;
			break;
		}

		list<pair<btTransform*, btCollisionShape*> > children = looseCompFather->getChildren();
		for(list<pair<btTransform*, btCollisionShape*> >::iterator child_it = children.begin();
					 child_it!=children.end();child_it++){
			child_it->first->setOrigin(child_it->first->getOrigin() + startTransform.getOrigin());
			btRigidBody* tempBody = addRigidBody(mass,*child_it->first,child_it->second,COL_CAKE,
					CAKE_COL_WITH);
			rigidBodies.push_back(tempBody);
			rigidObjectMap[tempBody] = m_objects.at(tempBody);
			((CakeObject*)m_objects.at(tempBody))->setParent(object);
			((CakeObject*)m_objects.at(tempBody))->setType(cakeType);
			pair<btRigidBody*, triangle> insertRigid;
			insertRigid.first = tempBody;
			insertRigid.second = ((CakeShape*)child_it->second)->getTri();
			rigidTrianglePairs.push_back(insertRigid);
		}
	}

	list<btVector3> innerPoints;
	for(list<triangle>::iterator tri_it = innerTriList.begin();
				 tri_it!=innerTriList.end();tri_it++){
		btVector3 current;
		current = (tri_it->p1 + tri_it->p2 + tri_it->p3)/3;
		innerPoints.push_back(current);
	}

	btTransform pointTrans;
	pointTrans.setIdentity();

	btCollisionShape* spartaPointShape = new btSphereShape(1);

	for(list<btVector3>::iterator point_it = innerPoints.begin();
			point_it != innerPoints.end(); point_it++){
		pointTrans.setOrigin(*point_it + startTransform.getOrigin());

		btRigidBody* tempBody = addRigidBody(mass,pointTrans,spartaPointShape,COL_SPARTA_POINTS,
				SPARTA_POINTS_COL_WITH, btVector3(0,1,0), .5);

		tempBody->setGravity(btVector3(0,0,0));
		Object* currObj = getObject(tempBody);
		rigidBodies.push_back(tempBody);
	}

	looseWall->setRigids(rigidBodies);
	looseWall->setCenterRigid(rigidWall);
	looseWall->setGravity(btVector3(0,0,0));


	object->setBorderPoints(points);
	object->setTriangles(triList);
	object->setFrame(framePoints);
	object->setDepth(wallDepth);
	object->setInnerTriangles(innerTriList);
	object->setRigids(rigidTrianglePairs);
//	printf("rigidTri size: %d\n", rigidTrianglePairs.size());
	object->setObjMap(rigidObjectMap);
//	printf("rigidObj size: %d\n", rigidObjectMap.size());

	btVector3 position;
	btTransform transform;
	transform.setIdentity();

	return looseWall;
}

LooseCompoundShape *ColoredWorld::addTriangles(const btTransform & startTransform,list<triangle> tris, float wallDepth, float linVel, CakeObject* object){
	LoadWall* loadWall = new LoadWall();

//	for(list<triangle>::iterator tri_it = tris.begin();
//			tri_it != tris.end(); tri_it++){
//		printf("tri:\n");
//		printf("%f %f %f\n", tri_it->p1.x(), tri_it->p1.y(), tri_it->p1.z());
//		printf("%f %f %f\n", tri_it->p2.x(), tri_it->p2.y(), tri_it->p2.z());
//		printf("%f %f %f\n", tri_it->p3.x(), tri_it->p3.y(), tri_it->p3.z());
//	}

	tris = loadWall->refineTriangles(tris,15);
	delete loadWall;

	ShapeFactory* fac = new ShapeFactory();
	btCollisionShape* wall = fac->getTriangleWall(tris, wallDepth);

	float mass = 1;

	LooseCompoundShape* looseWall = (LooseCompoundShape*) wall;

	list<pair<btTransform*, btCollisionShape*> > children = looseWall->getChildren();
	map<btRigidBody*, Object*> rigidObjectMap;
	list<pair<btRigidBody*, triangle> > childRigidTri;

	for(list<pair<btTransform*, btCollisionShape*> >::iterator child_it = children.begin();
				 child_it!=children.end();child_it++){

//		child_it->first->setOrigin(child_it->first->getOrigin() + startTransform.getOrigin());
		btRigidBody* tempBody = addRigidBody(mass,*child_it->first,child_it->second,COL_FINAL_CAKE,
				FINAL_CAKE_COL_WITH);
		tempBody->setGravity(btVector3(0,0,0));
		tempBody->setLinearVelocity(btVector3(0,0,linVel));
		rigidObjectMap[tempBody] = m_objects.at(tempBody);
		pair<btRigidBody*, triangle> insertPair;
		insertPair.first = tempBody;
		insertPair.second = ((CakeShape*)child_it->second)->getTri();

		childRigidTri.push_back(insertPair);
	}

	looseWall->setRigidTris(childRigidTri);
	object->setObjMap(rigidObjectMap);

	return looseWall;
}


void ColoredWorld::cleanup(){
	for (int i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );

		delete obj;
	}

}

Object* ColoredWorld::getObject(btRigidBody* rigidBody){
	Object* result = 0;

	if(m_objects.find(rigidBody) != m_objects.end()){
		result = m_objects.at(rigidBody);
	} else if(m_transparentobjects.find(rigidBody) != m_transparentobjects.end()) {
		result = m_transparentobjects.at(rigidBody);
	}

	return result;
}

Object* ColoredWorld::getObject(btCollisionObject* object){
//	printf("collmapsize: %d\n",m_collMap.size());
//	printf("max_size: %d\n",m_collMap.max_size());
	Object* result = m_collMap.at(object);
	return result;
}


void ColoredWorld::deleteRigidBody(btRigidBody* rigidBody){
	bool remove = true;
	btCollisionObject* col_obj;
	Object* object;
//	printf("delete in color\n");

	if(m_objects.find(rigidBody) != m_objects.end()){
		object = m_objects.at(rigidBody);
		col_obj = object->getCollObj();

		if(object->getType() != COL_CASTLE){
			delete object;
		}
		m_objects.erase(rigidBody);

	} else if(m_transparentobjects.find(rigidBody) != m_transparentobjects.end()) {
		object = m_transparentobjects.at(rigidBody);
		col_obj = object->getCollObj();
		delete object;
		m_transparentobjects.erase(rigidBody);
	} else {

		// the object is not there any more!
		// This is a very nasty glitch due to errors in WallObject implementation
//		printf("not here\n");
		remove = false;
	}

	if(remove){
		if(object->getType() == COL_LETTERS){
			m_letterObjects.remove(object);
		}
	}


	if(remove){
	//	printf("numObjs: %d\n", m_objects.size());
		m_collMap.erase(col_obj);
		m_dynamicsWorld->removeRigidBody(rigidBody);
		delete rigidBody;
	}
}

/**
 * When game is paused, nothing should move.
 */
void ColoredWorld::freeze(){
	for(map<btRigidBody*, Object*>::iterator obj_it = m_objects.begin();
			obj_it != m_objects.end(); obj_it++){
		if((*obj_it).second->getType() == COL_FLYING_OBJECTS){
			frozenObject insertObj;

			insertObj.body = obj_it->first;
			insertObj.linVel = obj_it->first->getLinearVelocity();
			insertObj.angVel = obj_it->first->getAngularVelocity();
			insertObj.gravity = obj_it->first->getGravity();

			m_frozenObjects.push_back(insertObj);
			(*obj_it).first->setLinearVelocity(btVector3(0,0,0));
			(*obj_it).first->setAngularVelocity(btVector3(0,0,0));
			(*obj_it).first->setGravity(btVector3(0,0,0));
		}
	}
}

void ColoredWorld::unfreeze(){
	for(list<frozenObject>::iterator frozen_it = m_frozenObjects.begin();
			frozen_it != m_frozenObjects.end(); frozen_it++){
		btRigidBody* body = frozen_it->body;
		body->setLinearVelocity(frozen_it->linVel);
		body->setAngularVelocity(frozen_it->angVel);
		body->setGravity(frozen_it->gravity);
	}

	m_frozenObjects.clear();
}

btDynamicsWorld *ColoredWorld::getDynamicsWorld()
{
    return m_dynamicsWorld;
}

std::list<Object*> ColoredWorld::getLetterObjects(){
	return m_letterObjects;
}

std::list<Object*> ColoredWorld::getObjects()
{
	list<Object*> object_list;

	for(map<btRigidBody*, Object*>::iterator obj_it = m_objects.begin();
			obj_it != m_objects.end(); obj_it++){
		object_list.push_back(obj_it->second);
	}

	for(map<btRigidBody*, Object*>::iterator obj_it = m_transparentobjects.begin();
			obj_it != m_transparentobjects.end(); obj_it++){
		object_list.push_back(obj_it->second);
	}

	return object_list;
}

