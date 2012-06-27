/*
 * LetterDrawer.h
 *
 *  Created on: May 17, 2011
 *      Author: hendrik
 */

#ifndef LETTERDRAWER_H_
#define LETTERDRAWER_H_

#include <iostream>
#include "btBulletDynamicsCommon.h"
#include "../LetterCompoundShape.h"

#include <list>

class LetterDrawer {
public:
	LetterDrawer();
	virtual ~LetterDrawer();

public:
	btCompoundShape* getLetters(std::string letters);

private:
	void addA(btCompoundShape* cshape, btVector3 offset);
	void addB(btCompoundShape* cshape, btVector3 offset);
	void addC(btCompoundShape* cshape, btVector3 offset);
	void addD(btCompoundShape* cshape, btVector3 offset);
	void addE(btCompoundShape* cshape, btVector3 offset);
	void addF(btCompoundShape* cshape, btVector3 offset);
	void addG(btCompoundShape* cshape, btVector3 offset);
	void addH(btCompoundShape* cshape, btVector3 offset);
	void addI(btCompoundShape* cshape, btVector3 offset);
	void addJ(btCompoundShape* cshape, btVector3 offset);
	void addK(btCompoundShape* cshape, btVector3 offset);
	void addL(btCompoundShape* cshape, btVector3 offset);
	void addM(btCompoundShape* cshape, btVector3 offset);
	void addN(btCompoundShape* cshape, btVector3 offset);
	void addO(btCompoundShape* cshape, btVector3 offset);
	void addP(btCompoundShape* cshape, btVector3 offset);
	void addQ(btCompoundShape* cshape, btVector3 offset);
	void addR(btCompoundShape* cshape, btVector3 offset);
	void addS(btCompoundShape* cshape, btVector3 offset);
	void addT(btCompoundShape* cshape, btVector3 offset);
	void addU(btCompoundShape* cshape, btVector3 offset);
	void addV(btCompoundShape* cshape, btVector3 offset);
	void addW(btCompoundShape* cshape, btVector3 offset);
	void addX(btCompoundShape* cshape, btVector3 offset);
	void addY(btCompoundShape* cshape, btVector3 offset);
	void addZ(btCompoundShape* cshape, btVector3 offset);

	void addRarrow(btCompoundShape* cshape, btVector3 offset);
	void addLarrow(btCompoundShape* cshape, btVector3 offset);



	btQuaternion* calculateRotation(btVector3* axis, btVector3* targetAxis);
};

#endif /* LETTERDRAWER_H_ */
