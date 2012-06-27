/*
 * HighscoreHandler.h
 *
 *  Created on: Jun 30, 2011
 *      Author: hendrik
 */

#ifndef HIGHSCOREHANDLER_H_
#define HIGHSCOREHANDLER_H_

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>

#include <list>
#include <iostream>
#include <stdexcept>

struct PlayerPoints{
	std::string name;
	int points;
};

class HighscoreHandler {
public:
	HighscoreHandler();
	virtual ~HighscoreHandler();

	std::list<PlayerPoints> readFile(std::string file) throw (std::runtime_error);
	void writeToXMLFile(std::string file, std::list<PlayerPoints> highscoreList);
	void addToHighscoreList(std::string file, std::string name, int points);
	bool reachedHighscore(std::string file, int points);


private:
	XMLCh* TAG_Player;
	XMLCh* ATTR_Name;
	XMLCh* ATTR_Points;
	xercesc::XercesDOMParser* m_configFileParser;

	std::list<PlayerPoints> sortHighscores(std::list<PlayerPoints> highscoreList);
};

#endif /* HIGHSCOREHANDLER_H_ */
