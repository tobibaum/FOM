/*
 * PointingSystem.h
 *
 *  Created on: May 29, 2011
 *      Author: Tobi
 */

#ifndef POINTINGSYSTEM_H_
#define POINTINGSYSTEM_H_

#include "SignalUser.h"

class PointingSystem: public SignalUser {

private:
	PointingSystem();
	virtual ~PointingSystem();
public:

	static PointingSystem* getInstance();
	static void destroy();

	void trigger(Message msg);
    int getPoints() const;
    void setPoints(int m_points);
    int getOverallPoints() const;

private:

    static PointingSystem* theInstance;

	int m_points;
	int m_overall_points;
	bool m_running;
};

#endif /* POINTINGSYSTEM_H_ */
