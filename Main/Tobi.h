/*
 * Tobi.h
 *
 *  Created on: May 23, 2011
 *      Author: Tobi
 */

#ifndef TOBI_H_
#define TOBI_H_

#include "Field.h"
#include "CollisionDetector.h"
#include "PointingSystem.h"
#include "MenuList.h"
//#include "ObjLoader/ObjLoader.h"
#include <list>

class Tobi: public Field {
public:
	Tobi();
	virtual ~Tobi();

	void trigger();

	void initWorld();

	virtual void renderOverlay();
private:
	void step();

private: /* members */

//	ObjLoader* m_objloader;

	std::list<triangle> m_triangles ;
	std::list<btVector3> m_points ;


};

#endif /* TOBI_H_ */
