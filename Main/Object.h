/*
 * Object.h
 *
 *  Created on: May 11, 2011
 *      Author: Tobi
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "btBulletDynamicsCommon.h"
#include "BodyParts.h"
#include <string>
#include <list>

/**
 * Every physical appearance on the field is an object. Besides the physical options of
 * a bullet shape, it can have a certain color and transparency (or some texture [later!]).
 */
class Object {
public:
	Object(btVector3 color, btCollisionObject* collObj, btScalar transparency, int type, btRigidBody* body);

	virtual ~Object();

    btCollisionObject *getCollObj() const;
    btVector3 getColor() const;
    btScalar getTransparency() const;
    void setCollObj(btCollisionObject *m_collObj);
    void setColor(btVector3 m_color);
    void setTransparency(btScalar m_transparency);
    int getType() const;
    bool getCollided() const;
    virtual void setCollided(bool m_collided);
    btVector3 getDeltaLinearVelocity();
    btRigidBody* getRigidBody();

    void addTexture(std::string filename);
    std::string getTexture();
    BodyParts::parts getBodyPart() const;
    void setBodyPart(BodyParts::parts m_bodyPart);
protected:
    Object();
private:
protected:
    btRigidBody *m_rigidBody;
    bool m_collided;
    btVector3 m_color;
    btScalar m_transparency;
private:
    BodyParts::parts m_bodyPart;

	btCollisionObject* m_collObj;
	int m_type;

	std::string m_textureFile;
};

#endif /* OBJECT_H_ */
