/*
 * SpartaConfiguration.cpp
 *
 *  Created on: Jul 4, 2011
 *      Author: hendrik
 */

#include "SpartaConfiguration.h"

using namespace std;

SpartaConfiguration::SpartaConfiguration() {
	// TODO Auto-generated constructor stub

}

SpartaConfiguration::~SpartaConfiguration() {
	// TODO Auto-generated destructor stub
}

SpartaLevel* SpartaConfiguration::getLevel(int level){
	SpartaLevel* result = 0;
	for(list<SpartaLevel*>::iterator level_it = m_levelList.begin();
			level_it != m_levelList.end(); level_it++){
		if((*level_it)->getLevelNumber() == level){
			result = *level_it;
		}
	}
	return result;
}

void SpartaConfiguration::setLevelList(std::list<SpartaLevel*> levelList)
{
    m_levelList = levelList;
}

std::list<SpartaLevel*> SpartaConfiguration::getLevelList() const
{
    return m_levelList;
}


