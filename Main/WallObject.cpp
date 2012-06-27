/*
 * WallObject.cpp
 *
 *  Created on: Jun 21, 2011
 *      Author: Tobi
 */

#include "WallObject.h"
#include "LoadWall.h"
#include "GlutStuff.h"
#include "Cleaner.h"
#include "CakeShape.h"
#include "SignalStation.h"
#include "ColoredWorld.h"

using namespace std;

WallObject::WallObject(btVector3 color, btCollisionObject* collObj, btScalar transparency, int type, btRigidBody* body)
: Object(color, collObj, transparency, type, body)
{
	// TODO Auto-generated constructor stub
	m_collided = false;
}

WallObject::~WallObject() {
	// TODO Auto-generated destructor stub
//	printf("delete wall\n");

//	for(map<btRigidBody*, Object*>::iterator map_it = m_objMap.begin();
//			map_it != m_objMap.end(); map_it++){
//		ColoredWorld::getInstance()->deleteRigidBody(map_it->first);
//	}
	m_objMap.clear();
	m_rigids.clear();
	m_triangles.clear();
	m_innerTriangles.clear();
	m_borderPoints.clear();
}

std::list<btVector3> WallObject::getBorderPoints() const
{
    return m_borderPoints;
}

std::list<triangle> WallObject::getTriangles() const
{
    return m_triangles;
}

void WallObject::setBorderPoints(std::list<btVector3> m_borderPoints)
{
    this->m_borderPoints = m_borderPoints;
}

void WallObject::setTriangles(std::list<triangle> m_triangles)
{
    this->m_triangles = m_triangles;
}

void WallObject::setFrame(float* frame){
	m_frame = frame;
}

void WallObject::renderCake(triangle tri, btVector3 color, bool final){

	btVector3 currentTri;

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(.1,.1,.1);
	glMatrixMode(GL_MODELVIEW);

	static const GLfloat	planex[]={1,0,0,0};
	static const GLfloat	planey[]={0,1,0,0};
	static const GLfloat	planez[]={0,0,1,0};
	glTexGenfv(GL_S,GL_OBJECT_PLANE,planex);
	glTexGenfv(GL_T,GL_OBJECT_PLANE,planey);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);

	glColor4f(color.getX(),color.getY(),color.getZ(),m_transparency);
//	glColor4f(0,.5,1,.5);

	glEnable(GL_TEXTURE_2D);

//	if(!final){
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("redStone.jpg"));

		glBegin(GL_TRIANGLE_STRIP);

			btVector3 curr;
			for(int i = 0 ; i < 4 ; i++){
				switch(i){
				case 0:
					curr = tri.p1;
					break;
				case 1:
					curr = tri.p2;
					break;
				case 2:
					curr = tri.p3;
					break;
				case 3:
					curr = tri.p1;
					break;

				}

				glTexCoord2f(0,curr.getX());
				glVertex3f(curr.getX(),curr.getY(),curr.getZ());
				glTexCoord2f(m_depth,curr.getX());
				glVertex3f(curr.getX(),curr.getY(),curr.getZ() - m_depth);
			}

		glEnd();
//	}

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);

	glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("wall.jpg"));


	glBegin(GL_TRIANGLES);
		currentTri = tri.p1;
		glVertex3f(currentTri.x(),currentTri.y(),currentTri.z());
		currentTri = tri.p2;
		glVertex3f(currentTri.x(),currentTri.y(),currentTri.z());
		currentTri = tri.p3;
		glVertex3f(currentTri.x(),currentTri.y(),currentTri.z());

		currentTri = tri.p1 + btVector3(0,0,-m_depth);
		glVertex3f(currentTri.x(),currentTri.y(),currentTri.z());
		currentTri = tri.p2 + btVector3(0,0,-m_depth);
		glVertex3f(currentTri.x(),currentTri.y(),currentTri.z());
		currentTri = tri.p3 + btVector3(0,0,-m_depth);
		glVertex3f(currentTri.x(),currentTri.y(),currentTri.z());
	glEnd();
}

/**
 * Render each cake piece of the wall, to have the destroyable looks.
 */
void WallObject::renderWall(){

	float zValue = m_rigidBody->getWorldTransform().getOrigin().getZ();
//	printf("renderwall\n");

	if(zValue > 40){
//		printf("zValue: %f", zValue);
		// delete the wall
//		printf("i get here twice\n");
//		printf("mapsize: %d\n",m_objMap.size());

		for(map<btRigidBody*, Object*>::iterator map_it = m_objMap.begin();
				map_it != m_objMap.end(); map_it++){
			ColoredWorld::getInstance()->deleteRigidBody(map_it->first);
		}
		m_rigids.clear();
		m_objMap.clear();
//		Cleaner::getInstance()->deleteBody(((LooseCompoundShape*)m_rigidBody->getCollisionShape())->getCenterRigid(),0);
		ColoredWorld::getInstance()->deleteRigidBody(m_rigidBody);

		Message msg;
		msg.messageType = Message::MSG_WALL_ERASED;
		SignalStation::getInstance()->sendMessage(msg);
		return;
	}

	glDisable(GL_CULL_FACE);
	btScalar m[16];

	list<Object*> objList = ColoredWorld::getInstance()->getObjects();
	list<Object*>::iterator list_it;

//	glColor4f(1,0,0,0);
	for(list<pair<btRigidBody*, triangle> >::iterator rigid_it = m_rigids.begin();
					rigid_it != m_rigids.end(); rigid_it++){


		if(m_objMap.find(rigid_it->first) != m_objMap.end()){
			list_it = find(objList.begin(), objList.end(), m_objMap.at(rigid_it->first));
			if(list_it == objList.end()){
				// The object got removed from the coloredWorld, remove it here too.
				m_objMap.erase(rigid_it->first);
				continue;
			}
		} else {
			continue;
		}

		btTransform trans = rigid_it->first->getWorldTransform();

		if(zValue != trans.getOrigin().getZ()){
			if(!m_objMap.at(rigid_it->first)->getCollided()){
				btVector3 orig = trans.getOrigin();
				orig.setZ(zValue);
				trans.setOrigin(orig);
			}
		}

//		trans.setOrigin(trans.getOrigin() + btVector3(0,0,50));

		trans.getOpenGLMatrix(m);

//		string looseComp = "LooseCompound";
//		if(looseComp.compare(rigid_it->first->getCollisionShape()->getName()) == 0){
//			// We have a looseCompound Shape here, meaning that its a multi-part cake,
//			// render each part of it.
//		}

		btVector3 color = m_objMap.at(rigid_it->first)->getColor();

		if(m_objMap.at(rigid_it->first)->getType() == CakeObject::CAKE_TYPE_HOLE){
			color = btVector3(0,1,0);
		}


//		btVector3 color = btVector3(1,1,1);
		glPushMatrix();
		btglMultMatrix(m);
		triangle tri = rigid_it->second;
		Object* curr_obj = ColoredWorld::getInstance()->getObject(rigid_it->first);
		bool final = false;
		if(curr_obj->getType() == COL_FINAL_CAKE) final = true;
		renderCake(tri,color, final);
		glPopMatrix();
	}

	glColor4f(1,1,1,0);

	glEnable(GL_CULL_FACE);
}

void WallObject::renderWallold(){

	btTransform transform =	m_rigidBody->getWorldTransform();
	btMatrix3x3 matrix = transform.getBasis();
	btVector3 orig = btVector3(0,0,0);//transform.getOrigin();
	btScalar m[16];
	transform.getOpenGLMatrix(m);

	glPushMatrix();
	btglMultMatrix(m);

	int frameDepth = m_depth;
	glDisable(GL_CULL_FACE);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(.1,.1,.1);
	glMatrixMode(GL_MODELVIEW);

	static const GLfloat	planex[]={1,0,0,0};
	static const GLfloat	planey[]={0,1,0,0};
	static const GLfloat	planez[]={0,0,1,0};
	glTexGenfv(GL_S,GL_OBJECT_PLANE,planex);
	glTexGenfv(GL_T,GL_OBJECT_PLANE,planey);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("redStone.jpg"));

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);


	glBegin(GL_TRIANGLE_STRIP);

		list<btVector3>::iterator vec_it;
		btVector3 curr;
		vec_it = m_borderPoints.begin();

		for(;vec_it != m_borderPoints.end(); vec_it++){
			curr = (*vec_it) + orig;
			glTexCoord2f(0,vec_it->getX());
			glVertex3f(curr.getX(),curr.getY(),curr.getZ());
			glTexCoord2f(frameDepth,vec_it->getX());
			glVertex3f(curr.getX(),curr.getY(),curr.getZ() + frameDepth);
		}
		vec_it = m_borderPoints.begin();
		curr = (*vec_it) + orig;
		glTexCoord2f(0,vec_it->getX());
		glVertex3f(curr.getX(),curr.getY(),curr.getZ());
		glTexCoord2f(frameDepth,vec_it->getX());
		glVertex3f(curr.getX(),curr.getY(),curr.getZ() + frameDepth);

	glEnd();

	glColor4f(1,1,1,0);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);

	glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("wall.jpg"));


	glBegin(GL_TRIANGLES);

	for(int i = 0;i<2;i++){
		switch(i){
		case 0:
			orig = btVector3(0,0,0);
			break;
		case 1:
			orig = btVector3(0,0,-frameDepth);
			break;
		}
		for(list<triangle>::iterator tri_it = m_triangles.begin();tri_it != m_triangles.end();tri_it++){

			btVector3 normal = btVector3(0,0,1);

			curr = tri_it->p1 + orig;
//			glNormal3f(normal.getX(),normal.getY(),normal.getZ());
			glVertex3f(curr[0],curr[1],curr[2] + frameDepth);
			curr = tri_it->p2 + orig;
//			glNormal3f(normal.getX(),normal.getY(),normal.getZ());
			glVertex3f(curr[0],curr[1],curr[2] + frameDepth);
			curr = tri_it->p3 + orig;
//			glNormal3f(normal.getX(),normal.getY(),normal.getZ());
			glVertex3f(curr[0],curr[1],curr[2] + frameDepth);
		}
	}
	glEnd();

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);

	glTexGenfv(GL_S,GL_OBJECT_PLANE,planex);
	glTexGenfv(GL_T,GL_OBJECT_PLANE,planez);

	glBindTexture(GL_TEXTURE_2D,m_textureManager->getTextureHandle("redStone.jpg"));

//	glBegin(GL_QUADS);
//
//		glVertex3f(m_frame[0], m_frame[1] - 1, 0);
//		glVertex3f(m_frame[0] + m_frame[2], m_frame[1] - 1, 0);
//		glVertex3f(m_frame[0] + m_frame[2], m_frame[1] - 1, frameDepth);
//		glVertex3f(m_frame[0], m_frame[1] - 1, frameDepth);
//
//		glVertex3f(m_frame[0], m_frame[1] + m_frame[3] - 1, 0);
//		glVertex3f(m_frame[0] + m_frame[2], m_frame[1] + m_frame[3] - 1, 0);
//		glVertex3f(m_frame[0] + m_frame[2], m_frame[1] + m_frame[3] - 1, frameDepth);
//		glVertex3f(m_frame[0], m_frame[1] + m_frame[3] - 1, frameDepth);
//
//	glEnd();
//
//	glTexGenfv(GL_S,GL_OBJECT_PLANE,planey);
//	glTexGenfv(GL_T,GL_OBJECT_PLANE,planez);
//
//	glBegin(GL_QUADS);
//		glVertex3f(m_frame[0], m_frame[1] - 1, 0);
//		glVertex3f(m_frame[0], m_frame[1] + m_frame[3] - 1, 0);
//		glVertex3f(m_frame[0], m_frame[1] + m_frame[3] - 1, frameDepth);
//		glVertex3f(m_frame[0], m_frame[1] - 1, frameDepth);
//
//		glVertex3f(m_frame[0] + m_frame[2], m_frame[1] - 1, 0);
//		glVertex3f(m_frame[0] + m_frame[2], m_frame[1] + m_frame[3] - 1, 0);
//		glVertex3f(m_frame[0] + m_frame[2], m_frame[1] + m_frame[3] - 1, frameDepth);
//		glVertex3f(m_frame[0] + m_frame[2], m_frame[1] - 1, frameDepth);
//
//	glEnd();

	glDisable(GL_TEXTURE_2D);
	glColor4f(0,.5,0,.5);

//	glBegin(GL_TRIANGLES);
//
//	for(int i = 0;i<2;i++){
//		switch(i){
//		case 0:
//			orig = btVector3(0,0,0);
//			break;
//		case 1:
//			orig = btVector3(0,0,-frameDepth);
//			break;
//		}
//
//		for(list<triangle>::iterator tri_it = m_innerTriangles.begin();tri_it != m_innerTriangles.end();tri_it++){
//
//			btVector3 normal = btVector3(0,0,1);
//
//			curr = tri_it->p1 + orig;
////			glNormal3f(normal.getX(),normal.getY(),normal.getZ());
//			glVertex3f(curr[0],curr[1],curr[2] + frameDepth);
//			curr = tri_it->p2 + orig;
////			glNormal3f(normal.getX(),normal.getY(),normal.getZ());
//			glVertex3f(curr[0],curr[1],curr[2] + frameDepth);
//			curr = tri_it->p3 + orig;
////			glNormal3f(normal.getX(),normal.getY(),normal.getZ());
//			glVertex3f(curr[0],curr[1],curr[2] + frameDepth);
//		}
//	}
//	glEnd();

	glEnable(GL_CULL_FACE);

	glPopMatrix();
}

float WallObject::getDepth() const
{
    return m_depth;
}

void WallObject::setDepth(float m_depth)
{
    this->m_depth = m_depth;
}

std::list<triangle> WallObject::getInnerTriangles() const
{
    return m_innerTriangles;
}

void WallObject::setInnerTriangles(std::list<triangle> m_innerTriangles)
{
    this->m_innerTriangles = m_innerTriangles;
}

std::list<std::pair<btRigidBody*,triangle> > WallObject::getRigids() const
{
    return m_rigids;
}

void WallObject::setRigids(std::list<std::pair<btRigidBody*,triangle> > m_rigids)
{
    this->m_rigids.assign(m_rigids.begin(),m_rigids.end());
}

void WallObject::addRigids(std::list<std::pair<btRigidBody*,triangle> > rigids){
//	printf("sizehere: %d\n", m_rigids.size());
	for(list<pair<btRigidBody*, triangle> >::iterator pair_it = rigids.begin();
			pair_it != rigids.end(); pair_it++){
		m_rigids.push_back(*pair_it);
	}
}

std::map<btRigidBody*,Object*> WallObject::getObjMap() const
{
    return m_objMap;
}

void WallObject::setObjMap(std::map<btRigidBody*,Object*> m_objMap)
{
    this->m_objMap = m_objMap;
}

void WallObject::addObjMap(std::map<btRigidBody*,Object*> objMap){
	for(map<btRigidBody*, Object*>::iterator pair_it = objMap.begin();
			pair_it != objMap.end(); pair_it++){
		m_objMap[pair_it->first] = pair_it->second;
	}
}
