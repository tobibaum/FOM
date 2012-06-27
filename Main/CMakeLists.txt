# This is basically the overall name of the project in Visual Studio this is the name of the Solution File


# For every executable you have with a main method you should have an add_executable line below.
# For every add executable line you should list every .cpp and .h file you have associated with that executable.  


# This is the variable for Windows.  I use this to define the root of my directory structure.
SET(GLUT_ROOT ${BULLET_PHYSICS_SOURCE_DIR}/Glut)
# You shouldn't have to modify anything below this line 
########################################################


subdirs(SignalSlot Menu GameConfigurations ObjLoader LoadWall)

INCLUDE_DIRECTORIES(
${BULLET_PHYSICS_SOURCE_DIR}/src ${BULLET_PHYSICS_SOURCE_DIR}/src/OpenGL /usr/include/nite /usr/include/ni /usr/include/SDL /usr/local/include/SDL /usr/local/lib SignalSlot Menu GameConfiguration /usr/include/festival /usr/include/speech_tools ObjLoader LoadWall GameConfigurations ${CMAKE_SOURCE_DIR}/xerces-c-3.1.1/src/
${BULLET_PHYSICS_SOURCE_DIR}/src ${BULLET_PHYSICS_SOURCE_DIR}/src/OpenGL ..  ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/imgproc/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/highgui/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/core/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/flann/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/video/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/features2d/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/objdetect/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/calib3d/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/ml/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/contrib/include ${CMAKE_SOURCE_DIR}/OpenCV-2.3.0/modules/legacy/include /usr/include
)


LINK_LIBRARIES(
 ${CMAKE_SOURCE_DIR}/src/OpenGL/libOpenGLSupport.a  BulletDynamics  BulletCollision LinearMath  ${GLUT_glut_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} SignalSlot Menu GameConfiguration ObjLoader LoadWall
)

LINK_LIBRARIES(OpenNI pthread SDL_image SDL SDL_mixer)

ADD_EXECUTABLE(FOM
	main.cpp
	Field.cpp 
	Field.h		
	Cannon.cpp
	Cannon.h
	Player.h
	Player.cpp
	Skeleton.cpp
	Skeleton.h
	Castle.h
	Castle.cpp
	CastleObject.cpp
	CastleObject.h
	CastlePointCounter.cpp
	CastlePointCounter.h
	Colorer.cpp
	Colorer.h
	Tobi.h
	Tobi.cpp
	Hendrik.h
	Hendrik.cpp
	CollisionDetector.cpp
	CollisionDetector.h
	Object.h
	Object.cpp
	ColoredWorld.h
	ColoredWorld.cpp
	PointingSystem.h
	PointingSystem.cpp
	LooseCompoundShape.h
	LooseCompoundShape.cpp
	CakeShape.h
	CakeShape.cpp
	LetterCompoundShape.h
	LetterCompoundShape.cpp
	Camera.h
	Camera.cpp
	FlyingObject.cpp
	FlyingObject.h
	Cleaner.h
	Cleaner.cpp
#	SpeechSynch.h
#	SpeechSynch.cpp
	SoundManager.cpp
	SoundManager.h
	ShapeFactory.cpp
	ShapeFactory.h
	WallObject.cpp
	WallObject.h
	CakeObject.h
	CakeObject.cpp
	SpartaPointObject.cpp
	SpartaPointObject.h
	HandTracker.h
	HandTracker.cpp
	HighscoreHandler.h
	HighscoreHandler.cpp
	LevelHandler.h
	LevelHandler.cpp
	SpartaCannon.cpp
	SpartaCannon.h
)

add_dependencies(FOM OpenGLSupport)
