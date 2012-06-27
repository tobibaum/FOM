/*
 * PhysicalObject.h
 *
 *  Created on: May 28, 2011
 *      Author: Tobi
 */

#ifndef PHYSICALOBJECT_H_
#define PHYSICALOBJECT_H_

#include "SignalStation.h"
#include "../Message.h"

class SignalStation;



class SignalUser {
public:
	SignalUser();
	virtual ~SignalUser();

	void sendMessage(Message msg);

	virtual void trigger(Message msg) {};

protected:
	void setTrigger(Message::MessageType type);

private:
	SignalStation* m_theStation;
};

#endif /* PHYSICALOBJECT_H_ */
