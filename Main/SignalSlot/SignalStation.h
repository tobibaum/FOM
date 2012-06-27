/*
 * SignalStation.h
 *
 *  Created on: May 28, 2011
 *      Author: Tobi
 */

#ifndef SIGNALSTATION_H_
#define SIGNALSTATION_H_

#include <list>
#include <map>
#include "SignalUser.h"
#include "../Message.h"

class SignalUser;


class SignalStation {
private:
	SignalStation();
	virtual ~SignalStation();

public:
	static SignalStation* getInstance();
	static void destroy();

	void sendMessage(Message msg);

	void registerSlot(Message::MessageType msg, SignalUser* obj);

	void process();

private:
	static SignalStation* m_theInstance;

	std::list<Message> m_queue;
	std::map<Message::MessageType, std::list<SignalUser*> > m_slots;
};

#endif /* SIGNALSTATION_H_ */
