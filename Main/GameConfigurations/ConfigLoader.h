/*
 * ConfigLoader.h
 *
 *  Created on: Jun 23, 2011
 *      Author: hendrik
 */

#ifndef CONFIGLOADER_H_
#define CONFIGLOADER_H_

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

#include <list>
#include <map>
#include <iostream>
#include <stdexcept>

#include "../ShapeFactory.h"

#include "ArcadeConfiguration.h"
#include "SpartaConfiguration.h"


enum {
   ERROR_ARGS = 1,
   ERROR_XERCES_INIT,
   ERROR_PARSE,
   ERROR_EMPTY_DOCUMENT
};

class ConfigLoader {
	enum nodeType{
		NODE_ROOT = 0,
		NODE_MODE,
		NODE_LEVEL,
		NODE_CANNON_PROPERTIES,
		NODE_SPARTA_PROPERTIES,
		NODE_SHAPES,
		NODE_WALLS,
		NODE_WALL,
		NODE_SHAPE,
		NODE_WIN_CONDITION
	};
public:
	ConfigLoader();
	virtual ~ConfigLoader();

	ArcadeConfiguration* readConfigFile(std::string&) throw(std::runtime_error);
	SpartaConfiguration* readSpartaFile(std::string&) throw(std::runtime_error);

private:
	void initMap();
	void getLevelData(xercesc::DOMElement* element, ArcadeLevel* level);
	void getSpartaData(xercesc::DOMElement* element, SpartaLevel* level);
	void getShapes(xercesc::DOMElement* element, std::list<ShapeProbability*>* shapeList);
	void getWalls(xercesc::DOMElement* element, std::list<WallProbability*>* wallList);
	btVector3* ParseVector(std::string vecString);
	xercesc::XercesDOMParser* m_configFileParser;

	std::map<std::string,nodeType> nodeMap;
	std::map<std::string,ShapeFactory::shapeTypes> shapeMap;

	XMLCh* TAG_root;
	XMLCh* TAG_Level;
	XMLCh* TAG_Shapes;
	XMLCh* TAG_Shape;
	XMLCh* ATTR_ShapeName;
	XMLCh* ATTR_ShapeProbability;
	XMLCh* ATTR_LevelNumber;
	XMLCh* ATTR_CannonFrequency;
	XMLCh* ATTR_CannonSpeed;
	XMLCh* ATTR_CannonPosition;
	XMLCh* ATTR_CannonTarget;
	XMLCh* ATTR_GameMode;
	XMLCh* ATTR_ProbGood;
	XMLCh* ATTR_ProbBad;
	XMLCh* ATTR_ProbReflect;
	XMLCh* ATTR_WallSpeed;
	XMLCh* ATTR_WallName;
	XMLCh* ATTR_WallProbability;
	XMLCh* ATTR_WallFrequency;
	XMLCh* ATTR_StartPosition;
	XMLCh* TAG_CannonProperties;
	XMLCh* TAG_SpartaProperties;
	XMLCh* TAG_GameConditions;
	XMLCh* ATTR_PointThreshold;
	XMLCh* ATTR_TimeThreshold;
	XMLCh* ATTR_BallThreshold;
	XMLCh* ATTR_LifeThreshold;

	ShapeFactory m_shapeFactory;
};

#endif /* CONFIGLOADER_H_ */
