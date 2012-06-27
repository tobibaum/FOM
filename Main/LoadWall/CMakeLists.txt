# This is basically the overall name of the project in Visual Studio this is the name of the Solution File


# For every executable you have with a main method you should have an add_executable line below.
# For every add executable line you should list every .cpp and .h file you have associated with that executable.  


# This is the variable for Windows.  I use this to define the root of my directory structure.
SET(GLUT_ROOT ${BULLET_PHYSICS_SOURCE_DIR}/Glut)
# You shouldn't have to modify anything below this line 
########################################################

INCLUDE_DIRECTORIES(
${BULLET_PHYSICS_SOURCE_DIR}/src ${BULLET_PHYSICS_SOURCE_DIR}/src/OpenGL ..  ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/imgproc/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/highgui/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/core/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/flann/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/video/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/features2d/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/objdetect/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/calib3d/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/ml/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/contrib/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/legacy/include /usr/include
)


LINK_LIBRARIES(
 ${CMAKE_SOURCE_DIR}/src/OpenGL/libOpenGLSupport.a  BulletDynamics  BulletCollision LinearMath 
#cv cxcore highgui
opencv_core opencv_highgui opencv_imgproc
)


ADD_LIBRARY(LoadWall
	LoadWall.h
	LoadWall.cpp
)
