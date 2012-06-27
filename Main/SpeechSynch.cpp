/*
 * SpeechSynch.cpp
 *
 *  Created on: Jun 11, 2011
 *      Author: Tobi
 */

#include "SpeechSynch.h"

SpeechSynch::SpeechSynch() {
	festival_initialize(1,210000);
//	This must be called before any other festival functions may be called. It sets up the
//synthesizer system. The first argument if true, causes the system set up files to be loaded
//(which is normallly what is necessary), the second argument is the initial size of the Scheme heap,
//this should normally be 210000 unless you envisage processing very large Lisp structures.

	festival_eval_command("(voice_el_diphone)");
	festival_say_text("Welcome to Flying Object Madness");
//
//	festival_wait_for_spooler();

	setTrigger(Message::MSG_COL_PLAYER_LETTER);


	// Say some text;
//	festival_say_text("hello world");

}

SpeechSynch::~SpeechSynch() {
	// TODO Auto-generated destructor stub
}


void SpeechSynch::trigger(Message msg){
	festival_say_text(msg.menuPointWord.c_str());

}
