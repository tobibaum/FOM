/*
 * SpeechSynch.h
 *
 *  Created on: Jun 11, 2011
 *      Author: Tobi
 */

#ifndef SPEECHSYNCH_H_
#define SPEECHSYNCH_H_

#include <festival/festival.h>
#include "SignalSlot/SignalUser.h"

class SpeechSynch : public SignalUser{
public:
	SpeechSynch();
	virtual ~SpeechSynch();

	void trigger(Message msg);
};

#endif /* SPEECHSYNCH_H_ */
