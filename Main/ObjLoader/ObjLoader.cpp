/*
 * ObjLoader.cpp
 *
 *  Created on: Jun 16, 2011
 *      Author: Tobi
 */

#include "ObjLoader.h"
#include "../../src/BulletCollision/CollisionShapes/btShapeHull.h"


using namespace ConvexDecomposition;
using namespace std;

ObjLoader::ObjLoader() {
	// TODO Auto-generated constructor stub

}

ObjLoader::~ObjLoader() {
	// TODO Auto-generated destructor stub
}

btCollisionShape* ObjLoader::loadObject(std::string filename, btScalar objectHeight)
{
	WavefrontObj wo;

	printf("load Obj\n");
	int tcount = wo.loadObj(filename.c_str());
	printf("loading done.\n");

	if(!tcount){
		printf("Error loading obj file %s\n",filename.c_str());
		// Handling
		return 0;
	} else {
		btTriangleMesh* trimesh = new btTriangleMesh();

		btVector3 localScaling(1.f,1.f,1.f);

		int i;
		for ( i=0;i<wo.mTriCount;i++)
		{
			int index0 = wo.mIndices[i*3];
			int index1 = wo.mIndices[i*3+1];
			int index2 = wo.mIndices[i*3+2];

			btVector3 vertex0(wo.mVertices[index0*3], wo.mVertices[index0*3+1],wo.mVertices[index0*3+2]);
			btVector3 vertex1(wo.mVertices[index1*3], wo.mVertices[index1*3+1],wo.mVertices[index1*3+2]);
			btVector3 vertex2(wo.mVertices[index2*3], wo.mVertices[index2*3+1],wo.mVertices[index2*3+2]);

			vertex0 *= localScaling;
			vertex1 *= localScaling;
			vertex2 *= localScaling;

			trimesh->addTriangle(vertex0,vertex1,vertex2);
		}


		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(0,2,14));

		btVector3 aabbMin, aabbMax;
		trimesh->calculateAabbBruteForce(aabbMin,aabbMax);
		float height = aabbMax.y() - aabbMin.y();

		float scaling = objectHeight/height;

		trimesh->setScaling(btVector3(scaling,scaling,scaling));

		bool useQuantization = false;
		btCollisionShape* concaveShape = new btBvhTriangleMeshShape(trimesh,useQuantization);

		return concaveShape;
//		return convexShape;

	}


}
