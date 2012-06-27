/*
 * SpartaConfiguration.h
 *
 *  Created on: Jul 4, 2011
 *      Author: hendrik
 */

#ifndef SPARTACONFIGURATION_H_
#define SPARTACONFIGURATION_H_

#include "SpartaLevel.h"
#include <list>

class SpartaConfiguration {
public:
	SpartaConfiguration();
	virtual ~SpartaConfiguration();
    void setLevelList(std::list<SpartaLevel*> levelList);

	SpartaLevel* getLevel(int level);
    std::list<SpartaLevel*> getLevelList() const;

private:
	std::list<SpartaLevel*> m_levelList;
};

#endif /* SPARTACONFIGURATION_H_ */
