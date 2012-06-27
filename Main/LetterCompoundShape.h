/*
 * LooseCompoundShape.h
 *
 *  Created on: May 31, 2011
 *      Author: hendrik
 */

#ifndef LETTERCOMPOUNDSHAPE_H_
#define LETTERCOMPOUNDSHAPE_H_
#include "btBulletDynamicsCommon.h"
#include <iostream>


class LetterCompoundShape: public btCompoundShape{
public:
	LetterCompoundShape();
	virtual ~LetterCompoundShape();
	std::string getType() const;
    void setType(std::string m_type);

private:
    std::string m_type;
};

#endif /* LOOSECOMPOUNDSHAPE_H_ */
