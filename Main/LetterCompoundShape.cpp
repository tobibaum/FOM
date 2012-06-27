/*
 * LooseCompoundShape.cpp
 *
 *  Created on: May 31, 2011
 *      Author: hendrik
 */

#include "LetterCompoundShape.h"

using namespace std;

LetterCompoundShape::LetterCompoundShape() {
	// TODO Auto-generated constructor stub

}

LetterCompoundShape::~LetterCompoundShape() {
	// TODO Auto-generated destructor stub
}

string LetterCompoundShape::getType() const
{
    return m_type;
}

void LetterCompoundShape::setType(string m_type)
{
    this->m_type = m_type;
}


