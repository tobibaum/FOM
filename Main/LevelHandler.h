/*
 * LevelHandler.h
 *
 *  Created on: Jul 4, 2011
 *      Author: hendrik
 */

#ifndef LEVELHANDLER_H_
#define LEVELHANDLER_H_

#include "SignalSlot/SignalUser.h"
#include "PointingSystem.h"
#include "LinearMath/btQuickprof.h"
#include "ArcadeConfiguration.h"
#include "SpartaConfiguration.h"

class LevelHandler : public SignalUser{
private:
	LevelHandler();
	virtual ~LevelHandler();

public:
	static LevelHandler* getInstance();
	static void destroy();

    int getCurrentLevel() const;
    void setCurrentLevel(int currentLevel);
    int getMaxLevels() const;
    void setMaxLevels(int maxLevels);

    void initialize();

    void trigger(Message msg);
    void resetStatistics();
    int getBadHits() const;
    int getGoodHits() const;
    int getReflectHits() const;

    int getBonusTime();
    int getBonusLifes();
    int getBonusBalls();

    void process();

    void setArcadeConfiguration(ArcadeConfiguration* config);
    void setSpartaConfiguration(SpartaConfiguration* config);
    int getBallsRemaining() const;
    int getLivesRemaining() const;
    int getPoints() const;
    int getTimeRemaining() const;
    void setLivesRemaining(int m_livesRemaining);
    std::list<std::string> getDescriptionString() const;

private:
    static LevelHandler* theInstance;

	int m_currentLevel;
	int maxLevels;
	int m_badHits;
	int m_goodHits;
	int m_reflectHits;
	int m_notHit;
	PointingSystem* m_pointingSystem;

	int m_timeRemaining;
	int m_ballsRemaining;
	int m_points;
	int m_livesRemaining;

	int m_bonusBalls;
	int m_bonusTime;
	int m_bonusLifes;

	float m_liveWeight;
	float m_ballWeight;
	float m_timeWeight;

	bool m_started;
	bool m_paused;

	std::list<std::string> m_descriptionString;

	ArcadeConfiguration::gameMode m_mode;

	SpartaConfiguration* m_spartaConfig;
	ArcadeConfiguration* m_arcadeConfig;

	int m_ballThresh;
	int m_lifeThresh;
	int m_pointThresh;
	int m_timeThresh;

	btScalar m_timePaused;

	btClock* m_clock;
	btClock* m_pauseClock;
};

#endif /* LEVELHANDLER_H_ */
