# This is basically the overall name of the project in Visual Studio this is the name of the Solution File


# For every executable you have with a main method you should have an add_executable line below.
# For every add executable line you should list every .cpp and .h file you have associated with that executable.  


# This is the variable for Windows.  I use this to define the root of my directory structure.
SET(GLUT_ROOT ${BULLET_PHYSICS_SOURCE_DIR}/Glut)
# You shouldn't have to modify anything below this line 
########################################################

INCLUDE_DIRECTORIES(
${BULLET_PHYSICS_SOURCE_DIR}/src ${BULLET_PHYSICS_SOURCE_DIR}/src/OpenGL .. 
)


LINK_LIBRARIES(
 ${CMAKE_SOURCE_DIR}/src/OpenGL/libOpenGLSupport.a  BulletDynamics  BulletCollision LinearMath 
)

ADD_LIBRARY(ObjLoader
	cd_wavefront.h
	cd_wavefront.cpp
	float_math.cpp
	float_math.h
	ObjLoader.cpp
	ObjLoader.h
)
