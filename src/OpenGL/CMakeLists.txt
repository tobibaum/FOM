# This is basically the overall name of the project in Visual Studio this is the name of the Solution File


# For every executable you have with a main method you should have an add_executable line below.
# For every add executable line you should list every .cpp and .h file you have associated with that executable.  


FIND_PACKAGE(SDL_image)

# You shouldn't have to modify anything below this line 
########################################################




INCLUDE_DIRECTORIES(
${BULLET_PHYSICS_SOURCE_DIR}/src ${BULLET_PHYSICS_SOURCE_DIR}/Extras/ConvexHull ${BULLET_PHYSICS_SOURCE_DIR}/Main/SignalSlot SDL_image SDL /usr/include/ni
)

		
ADD_LIBRARY(OpenGLSupport
		GLDebugFont.cpp
		GLDebugFont.h
		GL_DialogDynamicsWorld.cpp
		GL_DialogDynamicsWorld.h
		GL_DialogWindow.cpp
		GL_DialogWindow.h
		GL_ShapeDrawer.cpp
		GL_ShapeDrawer.h
		GL_Simplex1to4.cpp
		GL_Simplex1to4.h
		GLDebugDrawer.cpp
		GLDebugDrawer.h
		RenderTexture.cpp
		RenderTexture.h
		DemoApplication.cpp
		DemoApplication.h

		TextureManager.cpp
		TextureManager.h
				
		GlutDemoApplication.cpp
		GlutDemoApplication.h
		GlutStuff.cpp
		GlutStuff.h
)


  TARGET_LINK_LIBRARIES(OpenGLSupport BulletDynamics BulletCollision ${GLUT_glut_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} SDL_Image)

#INSTALL of other files requires CMake 2.6
IF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5)
	IF(INSTALL_EXTRA_LIBS)
		IF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
			INSTALL(TARGETS OpenGLSupport DESTINATION .)
		ELSE (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
		INSTALL(TARGETS OpenGLSupport DESTINATION lib)
			INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.h")
		ENDIF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
	ENDIF (INSTALL_EXTRA_LIBS)
ENDIF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5)

