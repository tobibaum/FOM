/*
 * LoadWall.h
 *
 *  Created on: Jun 20, 2011
 *      Author: Tobi
 */

#ifndef LOADWALL_H_
#define LOADWALL_H_


#ifndef CALLBACK
#define CALLBACK

#include "btBulletDynamicsCommon.h"
#include "GlutStuff.h"
//#include <opencv/cv.h>
//#include <opencv/cv.hpp>
//#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv/cxcore.h>
#include <opencv/cv.h>
#include "../ShapeFactory.h"


#include <string>
#include <list>

class LoadWall {
public:
	LoadWall();
	virtual ~LoadWall();

	std::list<btVector3> getPoints(std::string filename, float targetHeight);
	std::list<triangle> getTriangles(std::string filename, float targetHeight);
	std::list<triangle> getInnerTriangles(std::string filename, float targetHeight);
	float* getFrame(std::string filename, float targetHeight);

	std::list<triangle> refineTriangles(std::list<triangle> triangles, float thresh);

private:

	float calcTriSize(triangle tri);

	std::list<btVector3> getPoints(std::string filename, float targetHeight, bool with_border);
	float sqDistance(CvPoint a, btVector3 b);
	int findClosestPoint(CvPoint point, std::list<btVector3> pointList);

	void correctYCoordinate();
	void createTriangleDatabase(float thresh);

	std::list<triangle> triangleFromRectangle(std::list<CvRect> rectangles);

	static void CALLBACK tessBeginCB(GLenum which);
	static void CALLBACK tessEndCB();
	static void CALLBACK tessErrorCB(GLenum errorCode);
	static void CALLBACK tessVertexCB(const GLvoid *data);
	static void CALLBACK tessCombineCB(GLdouble coords[3], void *vertex_data[4],
            GLfloat weight[4], void **outData);

	std::map<std::string, std::list<triangle> > m_triangleLists;
	std::map<std::string, std::list<triangle> > m_innerTriangleLists;
	std::map<std::string, std::list<btVector3> > m_pointLists;
	std::map<std::string, float* > m_frame;


	static std::list<triangle> m_staticTriangleList;
	static std::list<btVector3> m_staticPointList;
	static std::list<btVector3> m_staticLastVectors;
	static GLenum m_currentMode;
	static float m_staticFrame[4];

	std::map<triangle, std::list<triangle> > m_smallTriangleDatabase;

//	static GLdouble vertices[64][6];
//	static int vertexIndex;

};

#endif
#endif /* LOADWALL_H_ */
