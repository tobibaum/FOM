/*
 * HighscoreHandler.cpp
 *
 *  Created on: Jun 30, 2011
 *      Author: hendrik
 */

#include "HighscoreHandler.h"
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include <xercesc/framework/LocalFileFormatTarget.hpp>

using namespace std;
XERCES_CPP_NAMESPACE_USE

HighscoreHandler::HighscoreHandler() {
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
	TAG_Player = XMLString::transcode("Player");
	ATTR_Name = XMLString::transcode("Name");
	ATTR_Points = XMLString::transcode("Points");

	m_configFileParser = new XercesDOMParser;

}

HighscoreHandler::~HighscoreHandler() {
	// TODO Auto-generated destructor stub
}

list<PlayerPoints> HighscoreHandler::readFile(string file) throw (std::runtime_error){
	struct stat fileStatus;

			   int iretStat = stat(file.c_str(), &fileStatus);
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

			   list<PlayerPoints> highscoreList;
			   try
			   {
			      m_configFileParser->parse( file.c_str() );

			      // no need to free this pointer - owned by the parent parser object
			      DOMDocument* xmlDoc = m_configFileParser->getDocument();

			      // Get the top-level element: NAme is "root". No attributes for "root"

			      DOMElement* elementRoot = xmlDoc->getDocumentElement();
			      if( !elementRoot ) throw(std::runtime_error( "empty XML document" ));

			      /* Here comes now the important stuff for reading the GameConfig files*/

			      DOMNodeList*      children = elementRoot->getChildNodes();
			      XMLSize_t nodeCount = children->getLength();

			      for(XMLSize_t i = 0; i < nodeCount; i++){

			    	  DOMNode* currentNode = children->item(i);
			    	  if( currentNode->getNodeType() &&  // true is not NULL
						 currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
					 {
						// Found node which is an Element. Re-cast node as element
						DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );

						if(XMLString::equals(currentElement->getTagName(), TAG_Player)){
							PlayerPoints pp;
							const XMLCh* xml_name = currentElement->getAttribute(ATTR_Name);
							string name = XMLString::transcode(xml_name);
							pp.name = name;

							const XMLCh* xml_points = currentElement->getAttribute(ATTR_Points);
							string points = XMLString::transcode(xml_points);
							stringstream ss(points);
							ss >> pp.points;

							highscoreList.push_back(pp);
						}

					 }

			      } // FOR
			      return highscoreList;


			   }
			   catch( xercesc::XMLException& e )
			   {
				  char* message = xercesc::XMLString::transcode( e.getMessage() );
				  ostringstream errBuf;
				  errBuf << "Error parsing file: " << message << flush;
				  XMLString::release( &message );
			   }


}

list<PlayerPoints> HighscoreHandler::sortHighscores(list<PlayerPoints> highscoreList){
	list<PlayerPoints> newList;

	if(highscoreList.size()>10){
		// Kleinsten Wert eliminieren
		int min = 9999999;
		list<PlayerPoints>::iterator min_pos;
		list<PlayerPoints>::iterator min_it;
		for(min_it = highscoreList.begin(); min_it != highscoreList.end(); min_it++){
			if(min > min_it->points){
				min = min_it->points;
				min_pos = min_it;
			}
		}
		highscoreList.erase(min_pos);
	}
	list<PlayerPoints>::iterator it;
	list<PlayerPoints>::iterator it_new;
	for(it = highscoreList.begin(); it != highscoreList.end(); it++){

		if(newList.empty()){
			newList.push_back(*it);
		}
		else{
			bool inserted=false;
			for(it_new = newList.begin(); it_new != newList.end(); it_new++){
				if((*it_new).points <= (*it).points){
					newList.insert(it_new,(*it));
					inserted = true;
					break;
				}
			}
			if(!inserted){newList.push_back(*it);}
		}
	}

	return newList;
}

void HighscoreHandler::addToHighscoreList(string file, string name, int points){
	PlayerPoints pp;
	pp.name = name;
	pp.points = points;

	list<PlayerPoints> newList = readFile(file);

	if(newList.empty()){
		newList.push_back(pp);
		writeToXMLFile(file, newList);
	}
	else{
		newList.push_back(pp);
		newList = sortHighscores(newList);
		writeToXMLFile(file, newList);
	}
}

void HighscoreHandler::writeToXMLFile(string file, list<PlayerPoints> highscoreList){
	// Initialize the XML4C2 system.
		printf("elements in highscoreList: %i\n", highscoreList.size());
	    try
	    {
	        XMLPlatformUtils::Initialize();
	    }

	    catch(const XMLException& toCatch)
	    {
	        char *pMsg = XMLString::transcode(toCatch.getMessage());
	        XERCES_STD_QUALIFIER cerr << "Error during Xerces-c Initialization.\n"
	             << "  Exception message:"
	             << pMsg;
	        XMLString::release(&pMsg);
	        return;
	    }

	    // Watch for special case help request
	    int errorCode = 0;

	   {

	       DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("Core"));

	       if (impl != NULL)
	       {
	           try
	           {
	               DOMDocument* doc = impl->createDocument(
	                           0,                    // root element namespace URI.
	                           XMLString::transcode("highscores"),         // root element name
	                           0);                   // document type object (DTD).

	               DOMElement* rootElem = doc->getDocumentElement();

	               list<PlayerPoints>::iterator it;
	               for(it = highscoreList.begin(); it != highscoreList.end(); it++){
	            	   DOMElement*  prodElem = doc->createElement(XMLString::transcode("Player"));
					   rootElem->appendChild(prodElem);
					   ostringstream ss;
					   ss << (it->points);
					   string pointString = ss.str();
					   prodElem->setAttribute(XMLString::transcode("Name"),XMLString::transcode((it->name).c_str()));
					   prodElem->setAttribute(XMLString::transcode("Points"),XMLString::transcode(pointString.c_str()));
	               }

	               //SERIALIZEN
	               const int ABSOLUTE_PATH_FILENAME_PREFIX_SIZE = 9;

	               DOMLSSerializer* serializer = ((DOMImplementationLS*)impl)->createLSSerializer();
				   if (serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
					   {serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);}

				   serializer->setNewLine(XMLString::transcode("\r\n"));

				   XMLCh* filePath = XMLString::transcode(file.c_str());

				   const int pathLen = XMLString::stringLen(filePath);

				   // Allocate memory for a Xerces string sufficent to hold the path.
				   XMLCh *targetPath = (XMLCh*)XMLPlatformUtils::fgMemoryManager->allocate((pathLen + ABSOLUTE_PATH_FILENAME_PREFIX_SIZE) * sizeof(XMLCh));

				   // Fixes a platform dependent absolute path filename to standard URI form.
				   XMLString::fixURI(filePath, targetPath);

				   // Specify the target for the XML output.
				   XMLFormatTarget *formatTarget = new LocalFileFormatTarget(targetPath);
				   //XMLFormatTarget *myFormTarget = new StdOutFormatTarget();

				   // Create a new empty output destination object.
				   DOMLSOutput *output = ((DOMImplementationLS*)impl)->createLSOutput();

				   // Set the stream to our target.
				   output->setByteStream(formatTarget);

				   // Write the serialized output to the destination.
				   serializer->write(doc, output);

				   // Cleanup.
				   serializer->release();
				   XMLString::release(&filePath);
				   delete formatTarget;
				   output->release();


	           }
	           catch (const OutOfMemoryException&)
	           {
	               XERCES_STD_QUALIFIER cerr << "OutOfMemoryException" << XERCES_STD_QUALIFIER endl;
	               errorCode = 5;
	           }
	           catch (const DOMException& e)
	           {
	               XERCES_STD_QUALIFIER cerr << "DOMException code is:  " << e.code << XERCES_STD_QUALIFIER endl;
	               errorCode = 2;
	           }
	           catch (...)
	           {
	               XERCES_STD_QUALIFIER cerr << "An error occurred creating the document" << XERCES_STD_QUALIFIER endl;
	               errorCode = 3;
	           }
	       }  // (inpl != NULL)
	       else
	       {
	           XERCES_STD_QUALIFIER cerr << "Requested implementation is not supported" << XERCES_STD_QUALIFIER endl;
	           errorCode = 4;
	       }
	   }
}

bool HighscoreHandler::reachedHighscore(string file, int points){
	list<PlayerPoints> highscoreList = readFile(file);
	bool enoughPoints = false;
	if(highscoreList.size()<10){return true;}
	list<PlayerPoints>::iterator it;
	for(it = highscoreList.begin(); it != highscoreList.end(); it++){
		if(it->points < points){
			enoughPoints = true;
			break;
		}
	}

	return enoughPoints;
}
