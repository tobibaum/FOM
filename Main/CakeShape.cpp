/*
 * CakeShape.cpp
 *
 *  Created on: Jul 1, 2011
 *      Author: Tobi
 */

#include "CakeShape.h"

CakeShape::CakeShape() {
	// TODO Auto-generated constructor stub

}

CakeShape::~CakeShape() {
	// TODO Auto-generated destructor stub
}

triangle CakeShape::getTri() const
{
    return m_tri;
}

void CakeShape::setTri(triangle m_tri)
{
    this->m_tri = m_tri;
}


