/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2007 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "Field.h"
#include "GlutStuff.h"
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btHashMap.h"
#include "Tobi.h"
#include "Hendrik.h"

#include "GLDebugDrawer.h"
static GLDebugDrawer sDebugDraw;

//#define DEBUG_WITHOUT_KINECT

Field* gameField;

void cleanup(){
	delete gameField;
}
	
int main(int argc,char** argv)
{


	bool error = false;
	if(argc != 1){
		if(argv[1][0] == 'h'){
			gameField = new Hendrik();
		} else if(argv[1][0] == 't'){
			gameField = new Tobi();
		} else {
			error = true;
		}
	} else {
		error = true;
	}

	if(error){
		printf("please use the run script in ../ \n");
		return 0;
	}


	if(!gameField->initPhysics()){
		// we were not succesfull!
#ifndef DEBUG_WITHOUT_KINECT
		return -1;
#endif
	}


	gameField->initWorld();

//	gameField.getDynamicsWorld()->setDebugDrawer(&sDebugDraw);

	atexit(cleanup);

#ifdef CHECK_MEMORY_LEAKS
	gameField.exitPhysics();
#else
	return glutmain(argc, argv,1358,668,"",gameField);
#endif
	
	//default glut doesn't return from mainloop
	return 0;
}

