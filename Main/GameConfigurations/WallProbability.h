/*
 * WallProbability.h
 *
 *  Created on: Jul 4, 2011
 *      Author: hendrik
 */

#ifndef WALLPROBABILITY_H_
#define WALLPROBABILITY_H_

#include <string>

class WallProbability {
public:
	WallProbability();
	virtual ~WallProbability();
    std::string getFilename() const;
    float getProbability() const;
    void setFilename(std::string filename);
    void setProbability(float probability);

private:
	std::string filename;
	float probability;
};

#endif /* WALLPROBABILITY_H_ */
