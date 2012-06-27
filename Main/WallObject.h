/*
 * WallObject.h
 *
 *  Created on: Jun 21, 2011
 *      Author: Tobi
 */

#ifndef WALLOBJECT_H_
#define WALLOBJECT_H_
#include "Object.h"
#include "ShapeFactory.h"
#include "LooseCompoundShape.h"
#include <list>
#include "../src/OpenGL/TextureManager.h"

class WallObject : public Object {
public:
	WallObject(btVector3 color, btCollisionObject* collObj, btScalar transparency, int type, btRigidBody* body);
	virtual ~WallObject();

	std::list<btVector3> getBorderPoints() const;
    std::list<triangle> getTriangles() const;
    void setBorderPoints(std::list<btVector3> m_borderPoints);
    void setTriangles(std::list<triangle> m_triangles);
    void setFrame(float* frame);

    void renderWall();
    void renderWallold();
    float getDepth() const;
    void setDepth(float m_depth);
    std::list<triangle> getInnerTriangles() const;
    void setInnerTriangles(std::list<triangle> m_innerTriangles);
    std::list<std::pair<btRigidBody*,triangle> > getRigids() const;
    void setRigids(std::list<std::pair<btRigidBody*,triangle> > m_rigids);
    void addRigids(std::list<std::pair<btRigidBody*,triangle> > rigids);
    std::map<btRigidBody*,Object*> getObjMap() const;
    void setObjMap(std::map<btRigidBody*,Object*> m_objMap);
    void addObjMap(std::map<btRigidBody*,Object*> objMap);

private:
    void renderCake(triangle tri,btVector3 color, bool final);

    std::list<triangle> m_triangles;
    std::list<triangle> m_innerTriangles;
	std::list<btVector3> m_borderPoints;
	float* m_frame;

	TextureManager* m_textureManager;

	std::list<std::pair<btRigidBody*, triangle> > m_rigids;
	float m_depth;

protected:
	std::map<btRigidBody*, Object*> m_objMap;


};

#endif /* WALLOBJECT_H_ */
