/*
 * LoadWall.cpp
 *
 *  Created on: Jun 20, 2011
 *      Author: Tobi
 */

#include "LoadWall.h"



using namespace std;

//int LoadWall::vertexIndex = 0;
GLenum LoadWall::m_currentMode;
list<btVector3> LoadWall::m_staticLastVectors;
list<triangle> LoadWall::m_staticTriangleList;
list<btVector3> LoadWall::m_staticPointList;
float LoadWall::m_staticFrame[4];

LoadWall::LoadWall() {

}

LoadWall::~LoadWall() {
	m_triangleLists.clear();
	m_innerTriangleLists.clear();
	m_pointLists.clear();
//	for(map<string, float* >::iterator frame_it = m_frame.begin();
//			frame_it != m_frame.end(); frame_it++) {
//		delete[] frame_it->second;
//	}

	m_staticTriangleList.clear();
	m_staticPointList.clear();
	m_staticLastVectors.clear();

//	delete[] m_staticFrame;

	m_smallTriangleDatabase.clear();


}

float LoadWall::sqDistance(CvPoint a, btVector3 b){
	return pow(a.x - b.getX(), 2) + pow(a.y - b.getY(), 2) ;
}

int LoadWall::findClosestPoint(CvPoint point, list<btVector3> pointList){
	float minDistance = 10000000;
	int minI = 0;

	int i=0;

	for(list<btVector3>::iterator point_it = pointList.begin();
			point_it != pointList.end(); point_it++){
		float curr = sqDistance(point,*point_it);
		if(minDistance > curr){
			minDistance = curr;
			minI = i;
		}
		i++;
	}

	return minI;
}

/**
 * Get the Triangulated Wall of the given file.
 * If getPoints has already been called for this instance, there won't be any
 * computations, they have already been performed, i.e., targetHeight is ig-
 * nored
 */
list<triangle> LoadWall::getTriangles(std::string filename, float targetHeight){
//	if(m_triangleLists.find(filename) == m_triangleLists.end()) {
//		getPoints(filename, targetHeight);
//		m_frame[filename] = m_staticFrame;
//		m_pointLists[filename] = m_staticPointList;
//		m_triangleLists[filename] = m_staticTriangleList;
//		getPoints(filename,targetHeight, false);
//		m_innerTriangleLists[filename] = m_staticTriangleList;

//	}
	return m_triangleLists.at(filename);
}

list<triangle> LoadWall::getInnerTriangles(std::string filename, float targetHeight){
//	if(m_innerTriangleLists.find(filename) == m_innerTriangleLists.end()){
//		getPoints(filename, targetHeight);
//		m_frame[filename] = m_staticFrame;
//		m_pointLists[filename] = m_staticPointList;
//		m_triangleLists[filename] = m_staticTriangleList;
//		getPoints(filename,targetHeight, false);
//		m_innerTriangleLists[filename] = m_staticTriangleList;
//	}

	return m_innerTriangleLists.at(filename);
}

float* LoadWall::getFrame(std::string filename, float targetHeight){
//	if(m_frame.find(filename) == m_frame.end()){
//		getPoints(filename, targetHeight);
//		m_frame[filename] = m_staticFrame;
//		m_pointLists[filename] = m_staticPointList;
//		m_triangleLists[filename] = m_staticTriangleList;
//		getPoints(filename,targetHeight, false);
//		m_innerTriangleLists[filename] = m_staticTriangleList;

//	}
	return m_staticFrame;
}

list<btVector3> LoadWall::getPoints(string filename, float targetHeight){

//	if(m_pointLists.size() == 0){
//		getPoints(filename, targetHeight, true);
//		m_frame[filename] = m_staticFrame;
//		m_pointLists[filename] = m_staticPointList;
//		m_triangleLists[filename] = m_staticTriangleList;
//		getPoints(filename,targetHeight, false);
//		m_innerTriangleLists[filename] = m_staticTriangleList;
//	}else if( m_pointLists.find(filename) == m_pointLists.end()){
	m_pointLists.clear();
	m_triangleLists.clear();
	m_innerTriangleLists.clear();

	getPoints(filename, targetHeight, true);
	m_frame[filename] = m_staticFrame;
	m_pointLists[filename] = m_staticPointList;
//		m_triangleLists[filename] = m_staticTriangleList;
	getPoints(filename,targetHeight, false);
//		m_innerTriangleLists[filename] = m_staticTriangleList;
//	}

	return m_pointLists.at(filename);
}



list<btVector3> LoadWall::getPoints(std::string filename, float targetHeight, bool with_border){
//	if(m_triangleList.size() != 0) return m_vectorList;

	m_staticLastVectors.clear();
	m_staticPointList.clear();
	m_staticTriangleList.clear();

	IplImage* inputImage = cvLoadImage(filename.c_str());

	if(!inputImage){
		printf("load failed: %s\n", filename.c_str());
		list<btVector3> empty;
		return empty;
	}

	IplImage* gray = cvCreateImage(cvGetSize(inputImage), inputImage->depth,1);

	cvCvtColor(inputImage,gray,CV_BGR2GRAY);
	cvConvertImage(gray,gray,CV_CVTIMG_FLIP);

	int imageWidth = inputImage->width;
	int imageHeight = inputImage->height;
	float wishedHeight = targetHeight;
	float wallScaling = wishedHeight/(float)imageHeight;

	int		g_thresh = 200;
	CvMemStorage* 	g_storage = NULL;

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvMemStorage* storage2 = cvCreateMemStorage(0);

	CvSeq* contours = 0;
	CvSeq* outline = 0;

	cvThreshold( gray, gray, g_thresh, 255, CV_THRESH_BINARY );


	cvFindContours( gray, storage, &contours,sizeof(CvContour), CV_RETR_TREE,
			CV_CHAIN_APPROX_NONE);


	contours = cvApproxPoly(contours,sizeof(CvContour),storage2, CV_POLY_APPROX_DP,2,1);

	CvSeq* hull = cvConvexHull2(contours);
	int hullcount = hull->total;
//	contours = hull;
//	printf("%d\n",hull->total);
	list<btVector3> contourList;
	list<btVector3> hullList;
	btVector3 current;
	int count = 0;

	float minX = 100000, maxX = -100000, minY = 100000,maxY = -100000;
	for( CvSeq* c=contours; c!=NULL; c=c->h_next ){
		for( int i=0; i < c->total; ++i ){
//			CvPoint* p = *CV_GET_SEQ_ELEM( CvPoint*, c, i );
			CvPoint* p = CV_GET_SEQ_ELEM( CvPoint, c, i );
			int x = (p->x-imageWidth/2)*wallScaling;
			current.setX(x);
			int y = p->y*wallScaling;
			current.setY(y);

			// compute the bounding box of the contour
			if(y < minY) minY = y;
			if(y > maxY) maxY = y;
			if(x < minX) minX = x;
			if(x > maxX) maxX = x;

			current.setZ(0);
			contourList.push_back(current);
			cvCircle(inputImage,*p,3,CV_RGB(255,0,0),1,2,0);
			count++;
		}
	}

	for( CvSeq* c=hull; c!=NULL; c=c->h_next ){
		for( int i=0; i < c->total; ++i ){
			CvPoint* p = *CV_GET_SEQ_ELEM( CvPoint*, c, i );

			int x = (p->x-imageWidth/2)*wallScaling;
			current.setX(x);
			int y = p->y*wallScaling;
			current.setY(y);
			current.setZ(0);
			hullList.push_back(current);
			cvCircle(inputImage,*p,3,CV_RGB(255,0,0),1,2,0);
			count++;
		}
	}


	contourList.reverse();
	hullList.reverse();


	GLdouble vec[512][3];

	GLUtesselator* tesselator = gluNewTess();

	gluTessCallback(tesselator, GLU_TESS_BEGIN, (void (*)(void))tessBeginCB);
	gluTessCallback(tesselator, GLU_TESS_END, (void (*)(void))tessEndCB);
	gluTessCallback(tesselator, GLU_TESS_ERROR, (void (*)(void))tessErrorCB);
	gluTessCallback(tesselator, GLU_TESS_VERTEX, (void (*)(void))tessVertexCB);
	gluTessCallback(tesselator, GLU_TESS_COMBINE, (void (*)(void))tessCombineCB);

	int tessNumber = 0;

//	imageWidth = 420;
//	imageHeight = maxY - minY;

//	printf("%fx%f: %fx%f\n", minX, minY,maxX, maxY);

	CvRect outerSizes = cvRect(-imageWidth/2*wallScaling,0,imageWidth*wallScaling,imageHeight*wallScaling);
	CvRect sizes = cvRect(minX,minY,maxX-minX,maxY-minY);

	m_staticFrame[0] = sizes.x;
	m_staticFrame[1] = sizes.y;
	m_staticFrame[2] = sizes.width;
	m_staticFrame[3] = sizes.height;

	gluTessBeginPolygon(tesselator,0);

	if(with_border ){
		gluTessBeginContour(tesselator);
			GLdouble frame[5][3];

			frame[0][0] = sizes.x;
			frame[0][1] = sizes.y;
			frame[0][2] = 0;
			gluTessVertex(tesselator,frame[0],frame[0]);

			frame[1][0] = sizes.x;
			frame[1][1] = sizes.y + sizes.height;
			frame[1][2] = 0;
			gluTessVertex(tesselator,frame[1],frame[1]);

			frame[2][0] = sizes.x + sizes.width;
			frame[2][1] = sizes.y + sizes.height;
			frame[2][2] = 0;
			gluTessVertex(tesselator,frame[2],frame[2]);

			frame[3][0] = sizes.x + sizes.width;
			frame[3][1] = sizes.y;
			frame[3][2] = 0;
			gluTessVertex(tesselator,frame[3],frame[3]);

		gluTessEndContour(tesselator);
	}
	list<btVector3>::iterator vec_it;

//		gluTessBeginContour(tesselator);
//			for(vec_it = hullList.begin();
//					vec_it != hullList.end(); vec_it++){
//
//				vec[tessNumber][0] = vec_it->x();
//				vec[tessNumber][1] = vec_it->y();
//				vec[tessNumber][2] = vec_it->z();
//				gluTessVertex(tesselator,vec[tessNumber],vec[tessNumber]);
//				tessNumber++;
//			}
//			vec_it = contourList.begin();
//			vec[tessNumber][0] = vec_it->x();
//			vec[tessNumber][1] = vec_it->y();
//			vec[tessNumber][2] = vec_it->z();
//			gluTessVertex(tesselator,vec[tessNumber],vec[tessNumber]);
//		gluTessEndContour(tesselator);

		gluTessBeginContour(tesselator);
			for(vec_it = contourList.begin();
				vec_it != contourList.end(); vec_it++){

			vec[tessNumber][0] = vec_it->x();
			vec[tessNumber][1] = vec_it->y();
			vec[tessNumber][2] = vec_it->z();
			gluTessVertex(tesselator,vec[tessNumber],vec[tessNumber]);
			tessNumber++;
		}
		vec_it = contourList.begin();
		vec[tessNumber][0] = vec_it->x();
		vec[tessNumber][1] = vec_it->y();
		vec[tessNumber][2] = vec_it->z();
		gluTessVertex(tesselator,vec[tessNumber],vec[tessNumber]);

		gluTessEndContour(tesselator);
	gluTessEndPolygon(tesselator);

	gluDeleteTess(tesselator);

//	printf("tessnumber: %d\n",tessNumber);
//	printf("tri-num: %d\n", m_staticTriangleList.size());

	// now add the outer triangles
	if(with_border){
		list<CvRect> insertRects;
		// left
		insertRects.push_back(cvRect(outerSizes.x,outerSizes.y, sizes.x - outerSizes.x,sizes.height + sizes.y));

		// right
		insertRects.push_back(cvRect(sizes.x + sizes.width,outerSizes.y, outerSizes.x + outerSizes.width - sizes.x - sizes.width,
				sizes.height + sizes.y));

		// top
		insertRects.push_back(cvRect(outerSizes.x,sizes.y + sizes.height, outerSizes.width,outerSizes.height-sizes.height));

		// bottom
		if(sizes.y != 0){
			insertRects.push_back(cvRect(sizes.x,outerSizes.y, sizes.width, sizes.y - outerSizes.y));
		}

		list<triangle> outerTris = triangleFromRectangle(insertRects);
		int outer = outerTris.size();

		if(sizes.y != 0){
			if(outer != 8){
//				printf("less than 8\n");
				outerTris = triangleFromRectangle(insertRects);
			}
		} else {
			if(outer != 6){
//				printf("less than 6\n");
				outerTris = triangleFromRectangle(insertRects);
			}
		}


		m_staticTriangleList.insert(m_staticTriangleList.end(),outerTris.begin(), outerTris.end());
	}

	m_staticPointList = contourList;

	correctYCoordinate();

	if(!with_border){
		// We now computed the inner triangles. we want those to be refined.
		list<triangle> insertThese = refineTriangles(m_staticTriangleList,15);
		m_innerTriangleLists[filename].assign(insertThese.begin(),insertThese.end());
	} else {
//		createTriangleDatabase(10);
		list<triangle> insertThese = refineTriangles(m_staticTriangleList,5000);
		m_triangleLists[filename].assign(insertThese.begin(),insertThese.end());
	}



//	printf("number of tris: %d\n", m_staticTriangleList.size());

	return m_staticPointList;
}

/**
 * the outer parts of the wall are going to be created with this.
 */
list<triangle> LoadWall::triangleFromRectangle(list<CvRect> rectangles){
	CvRect rect;
	list<triangle> result;
	triangle currentTri;

	for(list<CvRect>::iterator rect_it = rectangles.begin();
			rect_it != rectangles.end(); rect_it++){
		rect = *rect_it;
		currentTri.p1 = btVector3(rect.x,rect.y,0);
		currentTri.p2 = btVector3(rect.x + rect.width,rect.y,0);
		currentTri.p3 = btVector3(rect.x + rect.width,rect.y + rect.height,0);
		result.push_back(currentTri);

		currentTri.p3 = btVector3(rect.x,rect.y + rect.height,0);
		currentTri.p1 = btVector3(rect.x,rect.y,0);
		currentTri.p2 = btVector3(rect.x + rect.width,rect.y+rect.height,0);
		result.push_back(currentTri);
	}

//	result = refineTriangles(result, 50);

	return result;
}

/**
 * We create a database for the smaller triangles to look up, after a collision of
 * a bigger tri.
 */
void LoadWall::createTriangleDatabase(float thresh){

	m_smallTriangleDatabase.clear();

	for(list<triangle>::iterator tri_it = m_staticTriangleList.begin();
			tri_it != m_staticTriangleList.end(); tri_it++){

		if(m_smallTriangleDatabase.find(*tri_it) == m_smallTriangleDatabase.end()){
			list<triangle> theOneTri;
			theOneTri.push_back(*tri_it);
			list<triangle> newTris = refineTriangles(theOneTri,thresh);
			m_smallTriangleDatabase[*tri_it] = newTris;
		}
	}
}

void LoadWall::correctYCoordinate(){
	for(list<triangle>::iterator tri_it = m_staticTriangleList.begin();
			tri_it != m_staticTriangleList.end();tri_it++){
		tri_it->p1.setY(tri_it->p1.getY()-1);
		tri_it->p2.setY(tri_it->p2.getY()-1);
		tri_it->p3.setY(tri_it->p3.getY()-1);
	}

	for(list<btVector3>::iterator vec_it = m_staticPointList.begin();
			vec_it != m_staticPointList.end(); vec_it++){
		vec_it->setY(vec_it->y()-1);
	}
}

float LoadWall::calcTriSize(triangle tri){
	float lengthAB = tri.p1.distance(tri.p2);
	float lengthAC = tri.p1.distance(tri.p3);

	return (lengthAB*lengthAC)/2;

}

list<triangle> LoadWall::refineTriangles(list<triangle> triangles, float thresh){
	// This is for debug!!!!
//	return triangles;

//	if(triangles.size() == 1){
//		if(m_smallTriangleDatabase.find(triangles.front()) != m_smallTriangleDatabase.end()){
//			return m_smallTriangleDatabase.at(triangles.front());
//		}
//	}

	bool goOn = false;
	triangle current;
	list<triangle> smallTris;
	smallTris.clear();
	list<triangle> splitTris;
	splitTris.clear();
	list<triangle> newTris;
	newTris.clear();

//	printf("------------------------\nrefine\n");

	do{
		goOn = false;

		for(list<triangle>::iterator tri_it = triangles.begin();
				tri_it != triangles.end();tri_it++){
			current = *tri_it;

//			printf("trirefine:\n");
//			printf("%f %f %f\n", tri_it->p1.x(), tri_it->p1.y(), tri_it->p1.z());
//			printf("%f %f %f\n", tri_it->p2.x(), tri_it->p2.y(), tri_it->p2.z());
//			printf("%f %f %f\n", tri_it->p3.x(), tri_it->p3.y(), tri_it->p3.z());


//			printf("size: %f\n", calcTriSize(current));

			if(calcTriSize(current) > thresh){
				// there is a tri that is still to big, split it!
				float minLength = 100000;
				int index = -1;
				float length;

				for(int i = 0; i<3;i++){
					switch(i){
					case 0:
						length = current.p1.distance((current.p2 + current.p3)/2);
						break;
					case 1:
						length = current.p2.distance((current.p1 + current.p3)/2);
						break;
					case 2:
						length = current.p3.distance((current.p1 + current.p2)/2);
						break;
					}
					if(length < minLength){
						minLength = length;
						index = i;
					}
				}

				triangle insert_tri;
				switch(index){
				case 0:
					insert_tri.p1 = current.p1;
					insert_tri.p2 = current.p3;
					insert_tri.p3 = (current.p2 + current.p3)/2;
					splitTris.push_back(insert_tri);

					insert_tri.p1 = current.p1;
					insert_tri.p2 = current.p2;
					insert_tri.p3 = (current.p2 + current.p3)/2;
					splitTris.push_back(insert_tri);
					break;
				case 1:
					insert_tri.p1 = current.p2;
					insert_tri.p2 = current.p3;
					insert_tri.p3 = (current.p1 + current.p3)/2;
					splitTris.push_back(insert_tri);

					insert_tri.p1 = current.p2;
					insert_tri.p2 = current.p1;
					insert_tri.p3 = (current.p1 + current.p3)/2;
					splitTris.push_back(insert_tri);
					break;
				case 2:
					insert_tri.p1 = current.p3;
					insert_tri.p2 = current.p2;
					insert_tri.p3 = (current.p2 + current.p1)/2;
					splitTris.push_back(insert_tri);

					insert_tri.p1 = current.p3;
					insert_tri.p2 = current.p1;
					insert_tri.p3 = (current.p2 + current.p1)/2;
					splitTris.push_back(insert_tri);
					break;
				}

//				tri_it = triangles.erase(tri_it);

				goOn = true;
			}else {
				smallTris.push_back(*tri_it);
			}
		}

		triangles.assign(splitTris.begin(),splitTris.end());

		splitTris.clear();

//		printf("triangles size: %d\n", triangles.size());

	}while(goOn);

//	printf("split trees size: %d\n",splitTris.size());

	triangles.assign(smallTris.begin(), smallTris.end());

	return triangles;
}


/*
 * Tesselation Callbacks
 */

#define PRINT_GLENUM(type) \
		case type: \
		printf("" #type "\n"); \
		break;

void CALLBACK LoadWall::tessBeginCB(GLenum which)
{
//	switch(which){
//	PRINT_GLENUM(GL_TRIANGLES);
//	PRINT_GLENUM(GL_TRIANGLE_FAN);
//	PRINT_GLENUM(GL_TRIANGLE_STRIP);
//	default:
//		printf("other\n");
//		break;
//	}

    glBegin(which);

    m_currentMode = which;
    m_staticLastVectors.clear();

}

void CALLBACK LoadWall::tessEndCB()
{
	triangle insertTri;
	list<btVector3>::iterator vec_it;

	btVector3 firstPoint;
	btVector3 secondPoint;

//	printf("tesselation finished\n");

	switch(m_currentMode){
		case GL_TRIANGLE_FAN:
			vec_it = m_staticLastVectors.begin();
			firstPoint = *vec_it;
			vec_it++;
			secondPoint = *vec_it;

			for(;vec_it != m_staticLastVectors.end();vec_it++){
				insertTri.p1 = firstPoint;
				insertTri.p2 = *vec_it;
				vec_it++;
				insertTri.p3 = *vec_it;
				m_staticTriangleList.push_back(insertTri);
				vec_it++;

				if(vec_it == m_staticLastVectors.end()) {
					vec_it--;
					break;
				} else {
					vec_it--;
					vec_it--;
				}

			}

			break;
		case GL_TRIANGLE_STRIP:

			for(vec_it = m_staticLastVectors.begin();vec_it != m_staticLastVectors.end();vec_it++){
				insertTri.p1 = *vec_it;
				vec_it++;
				insertTri.p2 = *vec_it;
				vec_it++;
				insertTri.p3 = *vec_it;

				m_staticTriangleList.push_back(insertTri);
				vec_it++;
				if(vec_it == m_staticLastVectors.end()){
					break;
				}else{
					vec_it--;
					vec_it--;
					vec_it--;
				}

			}
			break;

		case GL_TRIANGLES:
			for(vec_it = m_staticLastVectors.begin();vec_it != m_staticLastVectors.end();vec_it++){
				insertTri.p1 = *vec_it;
				vec_it++;
				insertTri.p2 = *vec_it;
				vec_it++;
				insertTri.p3 = *vec_it;
				m_staticTriangleList.push_back(insertTri);
			}
			break;
		default:
			break;
	}

    glEnd();

}

void CALLBACK LoadWall::tessVertexCB(const GLvoid *data)
{
    // cast back to double type
    const GLdouble *ptr = (const GLdouble*)data;

    m_staticLastVectors.push_back(btVector3(ptr[0], ptr[1], ptr[2]));
}

void CALLBACK LoadWall::tessCombineCB(GLdouble coords[3], void *vertex_data[4],
        GLfloat weight[4], void **outData)
{

	// Well.....we just do nothing ^^

     *outData = 0;

//     printf("combine-----------\n");
//     printf("nu: %f %f %f\n",coords[0],coords[1],coords[2]);
//     printf("neigbors:\n");
//
//     printf("\t%f %f %f\n",((GLdouble*)vertex_data[0])[0],((GLdouble*)vertex_data[0])[1],((GLdouble*)vertex_data[0])[2]);
//     printf("\t%f %f %f\n",((GLdouble*)vertex_data[1])[0],((GLdouble*)vertex_data[1])[1],((GLdouble*)vertex_data[1])[2]);
//     printf("\t%f %f %f\n",((GLdouble*)vertex_data[2])[0],((GLdouble*)vertex_data[2])[1],((GLdouble*)vertex_data[2])[2]);
//     printf("\t%f %f %f\n",((GLdouble*)vertex_data[3])[0],((GLdouble*)vertex_data[3])[1],((GLdouble*)vertex_data[3])[2]);

}

void CALLBACK LoadWall::tessErrorCB(GLenum errorCode)
{
    const GLubyte *errorStr;

    errorStr = gluErrorString(errorCode);
    printf("Error in Tesselation\n");
    cerr << "[ERROR in Tesselation]: " << errorStr << endl;
}

