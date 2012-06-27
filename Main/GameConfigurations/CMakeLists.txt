# This is basically the overall name of the project in Visual Studio this is the name of the Solution File


# For every executable you have with a main method you should have an add_executable line below.
# For every add executable line you should list every .cpp and .h file you have associated with that executable.  


# This is the variable for Windows.  I use this to define the root of my directory structure.
SET(GLUT_ROOT ${BULLET_PHYSICS_SOURCE_DIR}/Glut)
# You shouldn't have to modify anything below this line 
########################################################

INCLUDE_DIRECTORIES(
${BULLET_PHYSICS_SOURCE_DIR}/src /usr/local/lib /usr/lib64 /lib64 /usr/lib ${BULLET_PHYSICS_SOURCE_DIR}/src/OpenGL .. ${CMAKE_SOURCE_DIR}/xerces-c-3.1.1/src/
)


LINK_LIBRARIES(
 ${CMAKE_SOURCE_DIR}/src/OpenGL/libOpenGLSupport.a  BulletDynamics  BulletCollision LinearMath  ${CMAKE_SOURCE_DIR}/xerces-c-3.1.1/src/.libs/libxerces-c-3.1.so
)


ADD_LIBRARY(GameConfiguration
	ArcadeConfiguration.cpp
	ArcadeConfiguration.h
	ConfigLoader.h
	ConfigLoader.cpp
	ArcadeLevel.h
	ArcadeLevel.cpp
	ShapeProbability.h
	ShapeProbability.cpp
	SpartaConfiguration.h
	SpartaConfiguration.cpp
	SpartaLevel.h
	SpartaLevel.cpp
	WallProbability.h
	WallProbability.cpp
)
