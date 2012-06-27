/*
 * ObjLoader.h
 *
 *  Created on: Jun 16, 2011
 *      Author: Tobi
 */

#ifndef OBJLOADER_H_
#define OBJLOADER_H_
#include <stdio.h>

#include <string>
#include "btBulletDynamicsCommon.h"
#include "cd_wavefront.h"

class ObjLoader {
public:
	ObjLoader();
	virtual ~ObjLoader();

	btCollisionShape* loadObject(std::string filename, btScalar objectHeight);

};

#endif /* OBJLOADER_H_ */
