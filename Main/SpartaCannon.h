/*
 * SpartaCanon.h
 *
 *  Created on: Jul 4, 2011
 *      Author: Tobi
 */

#ifndef SPARTACANNON_H_
#define SPARTACANNON_H_

#include "SpartaConfiguration.h"
#include "ColoredWorld.h"

class SpartaCannon: public SignalUser {
public:
	SpartaCannon();
	virtual ~SpartaCannon();

	void setConfiguration(SpartaConfiguration* config);

	void start();
	void stop();

	void trigger(Message msg);

	void updateLevelData(int level);

	void process();

private:
	btScalar get_time_delta_in_ms();

	SpartaConfiguration* m_config;
	float m_frequency;
	float m_speed;
	std::list<WallProbability*> m_probabilities;
	btVector3 m_startPosition;

	bool m_running;
	bool m_shoot;

	ColoredWorld* m_coloredWorld;

	LooseCompoundShape* m_currentWall;

	btScalar m_last_time;
	btScalar m_time_since_last_shot;
	btClock m_clock;
};

#endif /* SPARTACANON_H_ */
