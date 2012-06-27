/*
 * ConfigLoader.cpp
 *
 *  Created on: Jun 23, 2011
 *      Author: hendrik
 */

#define SPARTA 0
#define ARCADE 1

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "ConfigLoader.h"

using namespace xercesc;
using namespace std;

ConfigLoader::ConfigLoader() {
	try
   {
	  XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
   }
   catch( XMLException& e )
   {
	  char* message = XMLString::transcode( e.getMessage() );
	  cerr << "XML toolkit initialization error: " << message << endl;
	  XMLString::release( &message );
	  // throw exception here to return ERROR_XERCES_INIT
   }

   // Tags and attributes used in XML file.
   // Can't call transcode till after Xerces Initialize()
   TAG_root  = XMLString::transcode("root");
   TAG_Level = XMLString::transcode("Level");
   TAG_Shapes = XMLString::transcode("Shapes");
   TAG_Shape = XMLString::transcode("Shape");
   TAG_CannonProperties = XMLString::transcode("CannonProperties");
   TAG_SpartaProperties = XMLString::transcode("SpartaProperties");
   ATTR_ShapeName = XMLString::transcode("ShapeName");
   ATTR_ShapeProbability = XMLString::transcode("ShapeProbability");
   ATTR_LevelNumber = XMLString::transcode("LevelNumber");
   ATTR_CannonSpeed = XMLString::transcode("CannonSpeed");
   ATTR_CannonFrequency = XMLString::transcode("CannonFrequency");
   ATTR_CannonPosition = XMLString::transcode("CannonPosition");
   ATTR_WallSpeed = XMLString::transcode("WallSpeed");
   ATTR_WallFrequency = XMLString::transcode("WallFrequency");
   ATTR_WallName = XMLString::transcode("WallName");
   ATTR_WallProbability = XMLString::transcode("WallProbability");
   ATTR_StartPosition = XMLString::transcode("StartPosition");
   ATTR_CannonTarget = XMLString::transcode("CannonTarget");
   ATTR_GameMode = XMLString::transcode("GameMode");
   ATTR_ProbGood = XMLString::transcode("ProbabilityGood");
   ATTR_ProbBad = XMLString::transcode("ProbabilityBad");
   ATTR_ProbReflect = XMLString::transcode("ProbabilityReflect");
   TAG_GameConditions = XMLString::transcode("GameConditions");
   ATTR_PointThreshold = XMLString::transcode("PointThreshold");
   ATTR_BallThreshold = XMLString::transcode("BallThreshold");
   ATTR_TimeThreshold = XMLString::transcode("TimeThreshold");
   ATTR_LifeThreshold = XMLString::transcode("LifeThreshold");

   m_configFileParser = new XercesDOMParser;

   initMap();

}

ConfigLoader::~ConfigLoader() {
	delete m_configFileParser;

	try
	{
	   XMLPlatformUtils::Terminate();  // Terminate after release of memory
	}
	catch( xercesc::XMLException& e )
	{
    char* message = xercesc::XMLString::transcode( e.getMessage() );

	   cerr << "XML ttolkit teardown error: " << message << endl;
	   XMLString::release( &message );
	}
}

void ConfigLoader::initMap(){
	nodeMap["root"] = NODE_ROOT;
	nodeMap["Level"] = NODE_LEVEL;
	nodeMap["GameMode"] = NODE_MODE;
	nodeMap["Shapes"] = NODE_SHAPES;
	nodeMap["Walls"] = NODE_WALLS;
	nodeMap["Wall"] = NODE_WALL;
	nodeMap["Shape"] = NODE_SHAPE;
	nodeMap["GameConditions"] = NODE_WIN_CONDITION;
	nodeMap["CannonProperties"] = NODE_CANNON_PROPERTIES;
	nodeMap["SpartaProperties"] = NODE_SPARTA_PROPERTIES;

	shapeMap["BULLET"] = ShapeFactory::BULLET_SHAPE;
	shapeMap["CHAIR"] = ShapeFactory::CHAIR_SHAPE;
	shapeMap["KANTHOLZ"] = ShapeFactory::KANTHOLZ_SHAPE;
	shapeMap["CANNONBALL"] = ShapeFactory::CANNONBALL_SHAPE;
	shapeMap["SPIKEDBALL"] = ShapeFactory::SPIKEDBALL_SHAPE;
	shapeMap["PRESENT"] = ShapeFactory::PRESENT_SHAPE;
	shapeMap["WOOD"] = ShapeFactory::WOOD_SHAPE;
}

ArcadeConfiguration* ConfigLoader::readConfigFile(string & configFile) throw (std::runtime_error){

//	printf("i try to read the file\n");
	struct stat fileStatus;

		   int iretStat = stat(configFile.c_str(), &fileStatus);
		   if( iretStat == ENOENT )
		      throw ( std::runtime_error("Path file_name does not exist, or path is an empty string.") );
		   else if( iretStat == ENOTDIR )
		      throw ( std::runtime_error("A component of the path is not a directory."));
		   else if( iretStat == ELOOP )
		      throw ( std::runtime_error("Too many symbolic links encountered while traversing the path."));
		   else if( iretStat == EACCES )
		      throw ( std::runtime_error("Permission denied."));
		   else if( iretStat == ENAMETOOLONG )
		      throw ( std::runtime_error("File can not be read\n"));

		   // Configure DOM parser.

		   m_configFileParser->setValidationScheme( XercesDOMParser::Val_Never );
		   m_configFileParser->setDoNamespaces( false );
		   m_configFileParser->setDoSchema( false );
		   m_configFileParser->setLoadExternalDTD( false );

		   list<ArcadeLevel*> levelList;
		   ArcadeConfiguration* gameConfig;
		   int chosenMode;

		   try
		   {
		      m_configFileParser->parse( configFile.c_str() );

		      // no need to free this pointer - owned by the parent parser object
		      DOMDocument* xmlDoc = m_configFileParser->getDocument();

		      // Get the top-level element: NAme is "root". No attributes for "root"

		      DOMElement* elementRoot = xmlDoc->getDocumentElement();
		      if( !elementRoot ) throw(std::runtime_error( "empty XML document" ));

		      /* Here comes now the important stuff for reading the GameConfig files*/

		      DOMNodeList*      children = elementRoot->getChildNodes();
		      const  XMLSize_t nodeCount = children->getLength();
		      for(XMLSize_t i = 0; i < nodeCount; i++){

		    	  DOMNode* currentNode = children->item(i);
		    	  if( currentNode->getNodeType() &&  // true is not NULL
					 currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
				 {
					// Found node which is an Element. Re-cast node as element
					DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );

					nodeType type = nodeMap.at(XMLString::transcode(currentElement->getTagName()));

					if(type == NODE_MODE){
						const XMLCh* xml_name = currentElement->getAttribute(ATTR_GameMode);
						string mode = XMLString::transcode(xml_name);
						if(mode == "ARCADE"){
							gameConfig = new ArcadeConfiguration();
							chosenMode = ARCADE;
						}
					}

					if(type == NODE_LEVEL && chosenMode == ARCADE){
						ArcadeLevel* newLevel = new ArcadeLevel();
						const XMLCh* xml_name = currentElement->getAttribute(ATTR_LevelNumber);
						string levName = XMLString::transcode(xml_name);
						int i_levName;
						stringstream toInt(levName);
						toInt >> i_levName;
						newLevel->setLevelNumber(i_levName);

					    getLevelData(currentElement,newLevel);

					    levelList.push_back(newLevel);

					}
				 }

		      }
		      gameConfig->setLevelList(levelList);

		   }
		   catch( xercesc::XMLException& e )
		   {
			  char* message = xercesc::XMLString::transcode( e.getMessage() );
			  ostringstream errBuf;
			  errBuf << "Error parsing file: " << message << flush;
			  XMLString::release( &message );
		   }
	   return gameConfig;
}

SpartaConfiguration* ConfigLoader::readSpartaFile(string & configFile) throw (std::runtime_error){

//	printf("i try to read the file\n");
	struct stat fileStatus;

		   int iretStat = stat(configFile.c_str(), &fileStatus);
		   if( iretStat == ENOENT )
		      throw ( std::runtime_error("Path file_name does not exist, or path is an empty string.") );
		   else if( iretStat == ENOTDIR )
		      throw ( std::runtime_error("A component of the path is not a directory."));
		   else if( iretStat == ELOOP )
		      throw ( std::runtime_error("Too many symbolic links encountered while traversing the path."));
		   else if( iretStat == EACCES )
		      throw ( std::runtime_error("Permission denied."));
		   else if( iretStat == ENAMETOOLONG )
		      throw ( std::runtime_error("File can not be read\n"));

		   // Configure DOM parser.

		   m_configFileParser->setValidationScheme( XercesDOMParser::Val_Never );
		   m_configFileParser->setDoNamespaces( false );
		   m_configFileParser->setDoSchema( false );
		   m_configFileParser->setLoadExternalDTD( false );

		   list<SpartaLevel*> spartaList;
		   SpartaConfiguration* spartaConfig;
		   int chosenMode;

		   try
		   {
		      m_configFileParser->parse( configFile.c_str() );

		      // no need to free this pointer - owned by the parent parser object
		      DOMDocument* xmlDoc = m_configFileParser->getDocument();

		      // Get the top-level element: NAme is "root". No attributes for "root"

		      DOMElement* elementRoot = xmlDoc->getDocumentElement();
		      if( !elementRoot ) throw(std::runtime_error( "empty XML document" ));

		      /* Here comes now the important stuff for reading the GameConfig files*/

		      DOMNodeList*      children = elementRoot->getChildNodes();
		      const  XMLSize_t nodeCount = children->getLength();
//		      printf("%i\n", nodeCount);
		      for(XMLSize_t i = 0; i < nodeCount; i++){

		    	  DOMNode* currentNode = children->item(i);
		    	  if( currentNode->getNodeType() &&  // true is not NULL
					 currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
				 {
					// Found node which is an Element. Re-cast node as element
					DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );

					nodeType type = nodeMap.at(XMLString::transcode(currentElement->getTagName()));

					if(type == NODE_MODE){
						const XMLCh* xml_name = currentElement->getAttribute(ATTR_GameMode);
						string mode = XMLString::transcode(xml_name);

						if(mode == "SPARTA"){
							spartaConfig = new SpartaConfiguration();
							chosenMode = SPARTA;
						}
					}
					if(type == NODE_LEVEL && chosenMode == SPARTA){
						SpartaLevel* newLevel = new SpartaLevel();
						const XMLCh* xml_name = currentElement->getAttribute(ATTR_LevelNumber);
						string levName = XMLString::transcode(xml_name);
						int i_levName;
						stringstream toInt(levName);
						toInt >> i_levName;
						newLevel->setLevelNumber(i_levName);

						getSpartaData(currentElement,newLevel);

						spartaList.push_back(newLevel);
					}
				 }

		      }
		      spartaConfig->setLevelList(spartaList);

		   }
		   catch( xercesc::XMLException& e )
		   {
			  char* message = xercesc::XMLString::transcode( e.getMessage() );
			  ostringstream errBuf;
			  errBuf << "Error parsing file: " << message << flush;
			  XMLString::release( &message );
		   }
	   return spartaConfig;
}

void ConfigLoader::getLevelData(DOMElement* element, ArcadeLevel* level){
	DOMNodeList*      children = element->getChildNodes();
    const  XMLSize_t nodeCount = children->getLength();

    for(XMLSize_t i = 0; i < nodeCount; i++){

		  DOMNode* currentNode = children->item(i);
		  if( currentNode->getNodeType() &&  // true is not NULL
			 currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
		  {
			 // Found node which is an Element. Re-cast node as element
			 DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );

			 nodeType type = nodeMap.at(XMLString::transcode(currentElement->getTagName()));

			 if(type == NODE_CANNON_PROPERTIES){
//				 printf("    CANNON PROBS FOUND\n");
				 const XMLCh* xml_speed = currentElement->getAttribute(ATTR_CannonSpeed);
				 string speed = XMLString::transcode(xml_speed);
				 float f_speed;
				 stringstream toFloat(speed);
				 toFloat >> f_speed;
				 level->setShootSpeed(f_speed);

				 const XMLCh* xml_frequency = currentElement->getAttribute(ATTR_CannonFrequency);
				 string frequency = XMLString::transcode(xml_frequency);
				 float f_frequency;
				 stringstream toFloat2(frequency);
				 toFloat2 >> f_frequency;
				 level->setShootFrequency(f_frequency);

				 const XMLCh* xml_position = currentElement->getAttribute(ATTR_CannonPosition);
				 string position = XMLString::transcode(xml_position);
				 btVector3* pos = ParseVector(position);
				 level->setCanonPosition(*pos);

				 const XMLCh* xml_target = currentElement->getAttribute(ATTR_CannonTarget);
				 string target = XMLString::transcode(xml_target);
				 btVector3* tar = ParseVector(target);
				 level->setShootingTarget(*tar);

			}

			 if(type == NODE_SHAPES){
				 list<ShapeProbability*> shapeList;
				 getShapes(currentElement, &shapeList);
				 level->setShapes(shapeList);
			 }

			 if(type == NODE_WIN_CONDITION){
				 if(currentElement->hasAttribute(ATTR_PointThreshold)){
					const XMLCh* xml_pThresh = currentElement->getAttribute(ATTR_PointThreshold);
					string pThresh = XMLString::transcode(xml_pThresh);
					stringstream s1(pThresh);
					int i1;
					s1 >> i1;
					level->setPointThresh(i1);
				 } else {level->setPointThresh(0);}

				if(currentElement->hasAttribute(ATTR_BallThreshold)){
					const XMLCh* xml_hThresh = currentElement->getAttribute(ATTR_BallThreshold);
					string hThresh = XMLString::transcode(xml_hThresh);
					stringstream s2(hThresh);
					int i2;
					s2 >> i2;
					level->setBallThresh(i2);
				} else {level->setBallThresh(0);}

				if(currentElement->hasAttribute(ATTR_TimeThreshold)){
					const XMLCh* xml_tThresh = currentElement->getAttribute(ATTR_TimeThreshold);
					string tThresh = XMLString::transcode(xml_tThresh);
					stringstream s3(tThresh);
					int i3;
					s3 >> i3;
					level->setTimeThresh(i3);
				} else { level->setTimeThresh(0);}

				if(currentElement->hasAttribute(ATTR_LifeThreshold)){
					const XMLCh* xml_lThresh = currentElement->getAttribute(ATTR_LifeThreshold);
					string lThresh = XMLString::transcode(xml_lThresh);
					stringstream s4(lThresh);
					int i4;
					s4 >> i4;
					level->setLiveThresh(i4);
				} else { level->setLiveThresh(0);}
			}
		  }
    }
}

void ConfigLoader::getSpartaData(DOMElement* element, SpartaLevel* level){
	DOMNodeList*      children = element->getChildNodes();
    const  XMLSize_t nodeCount = children->getLength();

    for(XMLSize_t i = 0; i < nodeCount; i++){

		  DOMNode* currentNode = children->item(i);
		  if( currentNode->getNodeType() &&  // true is not NULL
			 currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
		  {
			 // Found node which is an Element. Re-cast node as element
			 DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );

			 nodeType type = nodeMap.at(XMLString::transcode(currentElement->getTagName()));

			 if(type == NODE_SPARTA_PROPERTIES){
				 const XMLCh* xml_speed = currentElement->getAttribute(ATTR_WallSpeed);
				 string speed = XMLString::transcode(xml_speed);
				 float f_speed;
				 stringstream toFloat(speed);
				 toFloat >> f_speed;
				 level->setSpeed(f_speed);

				 const XMLCh* xml_frequency = currentElement->getAttribute(ATTR_WallFrequency);
				 string frequency = XMLString::transcode(xml_frequency);
				 float f_frequency;
				 stringstream toFloat2(frequency);
				 toFloat2 >> f_frequency;
				 level->setFrequency(f_frequency);

				 const XMLCh* xml_position = currentElement->getAttribute(ATTR_StartPosition);
				 string position = XMLString::transcode(xml_position);
				 btVector3* pos = ParseVector(position);
				 level->setStartPosition(pos);

			}

			 if(type == NODE_WALLS){
				 list<WallProbability*> wallList;
				 getWalls(currentElement, &wallList);
				 level->setLevelList(wallList);
			 }
			 if(type == NODE_WIN_CONDITION){

				if(currentElement->hasAttribute(ATTR_BallThreshold)){
					const XMLCh* xml_hThresh = currentElement->getAttribute(ATTR_BallThreshold);
					string hThresh = XMLString::transcode(xml_hThresh);
					stringstream s2(hThresh);
					int i2;
					s2 >> i2;
					level->setBallThresh(i2);
				} else {level->setBallThresh(0);}
		     }
    }
}
}

void ConfigLoader::getShapes(DOMElement* element, list<ShapeProbability*>* shapeList){
	DOMNodeList*      children = element->getChildNodes();
	const  XMLSize_t nodeCount = children->getLength();

	for(XMLSize_t i = 0; i < nodeCount; i++){

		  DOMNode* currentNode = children->item(i);
		  if( currentNode->getNodeType() &&  // true is not NULL
			 currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
		  {
			 // Found node which is an Element. Re-cast node as element
			 DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );

			 nodeType type = nodeMap.at(XMLString::transcode(currentElement->getTagName()));

			 if(type == NODE_SHAPE){
				 ShapeProbability* shapeProb = new ShapeProbability();
				 const XMLCh* xml_name = currentElement->getAttribute(ATTR_ShapeName);
				 string name = XMLString::transcode(xml_name);
				 btCollisionShape* shape = m_shapeFactory.getShape(shapeMap.at(name));
				 shapeProb->setShape(shape);

				 const XMLCh* xml_prob = currentElement->getAttribute(ATTR_ShapeProbability);
				 string prob = XMLString::transcode(xml_prob);
				 float f_prob;
				 stringstream toFloat(prob);
				 toFloat >> f_prob;
				 shapeProb->setProbability(f_prob);

				 xml_prob = currentElement->getAttribute(ATTR_ProbGood);
				 prob = XMLString::transcode(xml_prob);
				 stringstream toFloat1(prob);
				 toFloat1 >> f_prob;
				 shapeProb->setProbGood(f_prob);

				 xml_prob = currentElement->getAttribute(ATTR_ProbBad);
				 prob = XMLString::transcode(xml_prob);
				 stringstream toFloat2(prob);
				 toFloat2 >> f_prob;
				 shapeProb->setProbBad(f_prob);

				 xml_prob = currentElement->getAttribute(ATTR_ProbReflect);
				 prob = XMLString::transcode(xml_prob);
				 stringstream toFloat3(prob);
				 toFloat3 >> f_prob;
				 shapeProb->setProbReflect(f_prob);

				 shapeList->push_back(shapeProb);


			 }
		  }
	}
}

void ConfigLoader::getWalls(DOMElement* element, list<WallProbability*>* wallList){
	DOMNodeList*      children = element->getChildNodes();
	const  XMLSize_t nodeCount = children->getLength();

	for(XMLSize_t i = 0; i < nodeCount; i++){

		  DOMNode* currentNode = children->item(i);
		  if( currentNode->getNodeType() &&  // true is not NULL
			 currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
		  {
			 // Found node which is an Element. Re-cast node as element
			 DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );

			 nodeType type = nodeMap.at(XMLString::transcode(currentElement->getTagName()));

			 if(type == NODE_WALL){
				 WallProbability* wallProb = new WallProbability();
				 const XMLCh* xml_name = currentElement->getAttribute(ATTR_WallName);
				 string name = XMLString::transcode(xml_name);
				 wallProb->setFilename(name);

				 const XMLCh* xml_prob = currentElement->getAttribute(ATTR_WallProbability);
				 string prob = XMLString::transcode(xml_prob);
				 float f_prob;
				 stringstream toFloat(prob);
				 toFloat >> f_prob;
				 wallProb->setProbability(f_prob);

				 wallList->push_back(wallProb);

			 }
		  }
	}
}

btVector3* ConfigLoader::ParseVector(string vecString){
	int start = 0;
	int end = 0;
	int xyzCounter=0;
	btScalar x,y,z;

	string number= "";

	char endsymbol = ',';

	while(end < vecString.length()){
		while(end < vecString.length() && vecString.at(end)!= endsymbol){
			number = number + vecString.at(end);
			end++;
		}
		stringstream toInt(number);
		switch(xyzCounter){
			case 0 : toInt >> x; break;
			case 1 : toInt >> y; break;
			case 2 : toInt >> z; break;
		}
		xyzCounter++;
		number = "";
		end++;
	}
	return new btVector3(x,y,z);
}
