/*
 * CakeShape.h
 *
 *  Created on: Jul 1, 2011
 *      Author: Tobi
 */

#ifndef CAKESHAPE_H_
#define CAKESHAPE_H_

#include "btBulletDynamicsCommon.h"
#include <iostream>
#include "Triangle.h"

class CakeShape : public btCompoundShape{
public:
	CakeShape();
	virtual ~CakeShape();
    triangle getTri() const;
    void setTri(triangle m_tri);

private:
	triangle m_tri;
};

#endif /* CAKESHAPE_H_ */
