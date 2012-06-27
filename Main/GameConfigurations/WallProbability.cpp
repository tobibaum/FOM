/*
 * WallProbability.cpp
 *
 *  Created on: Jul 4, 2011
 *      Author: hendrik
 */

#include "WallProbability.h"

WallProbability::WallProbability() {
	// TODO Auto-generated constructor stub

}

WallProbability::~WallProbability() {
	// TODO Auto-generated destructor stub
}

std::string WallProbability::getFilename() const
{
    return filename;
}

float WallProbability::getProbability() const
{
    return probability;
}

void WallProbability::setFilename(std::string filename)
{
    this->filename = filename;
}

void WallProbability::setProbability(float probability)
{
    this->probability = probability;
}


