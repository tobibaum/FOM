/*
 * TextureManager.cpp
 *
 *  Created on: Jun 6, 2011
 *      Author: Tobi
 */

#include "TextureManager.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

using namespace std;

map<string, unsigned int> TextureManager::m_texturehandles;

TextureManager::TextureManager() {
	// TODO Auto-generated constructor stub

}

TextureManager::~TextureManager() {
	m_texturehandles.clear();
	for(map<string, unsigned int>::iterator tex_it = m_texturehandles.begin();
			tex_it != m_texturehandles.end(); tex_it++){
		glDeleteTextures(1,(const GLuint*) &tex_it->second);
	}
}

unsigned int TextureManager::getTextureHandle(std::string filename){
	if(m_texturehandles.find(filename) == m_texturehandles.end()){
		// The texture is called for the first time, create it.
		loadTexture(filename);
	}
	return m_texturehandles.at(filename);
}

void TextureManager::loadTexture(string filename){
	m_texturehandles[filename] = 0;
	FILE * file;


	file = fopen(filename.c_str(), "rb");

//	if (file != NULL){
//
//		// TODO!!!
//		int width = 256;
//		int height = 256;
//
//		if(file==0){
//			printf("Could not load texture: %s\n",filename.c_str());
//		}
//		// allocate back buffer
//		GLubyte* image = (GLubyte*) malloc(width * height * 3);
//
//		// read data
//		fread(image, 1,width * height * 3,  file);
//		fclose(file);
//
//
//		glGenTextures(1,(GLuint*)&m_texturehandles.at(filename));
//		glBindTexture(GL_TEXTURE_2D,m_texturehandles.at(filename));
//		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
//		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
//		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
//		gluBuild2DMipmaps(GL_TEXTURE_2D,3,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);
//		delete[] image;
//
//		printf("new texture loaded\n");
//	}



	// ----------------------
	 SDL_Surface *surface;
	  GLenum texture_format;
	  GLint  nOfColors;

	  if ( (surface = IMG_Load(filename.c_str())) )
	  {
	    nOfColors = surface->format->BytesPerPixel;
	    if (nOfColors == 4)     // contains an alpha channel
	    {
	      if (surface->format->Rmask == 0x000000ff)
	        texture_format = GL_RGBA;
	      else
	        texture_format = GL_BGRA;

//	      printf("%s: yes\n", filename.c_str());
	    } else if (nOfColors == 3)     // no alpha channel
	    {
	      if (surface->format->Rmask == 0x000000ff)
	        texture_format = GL_RGB;
	      else
	        texture_format = GL_BGR;
//	      printf("%s: no\n", filename.c_str());
	    }
	    glGenTextures( 1, (GLuint*)&m_texturehandles.at(filename));
	    glBindTexture( GL_TEXTURE_2D, m_texturehandles.at(filename));
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	    glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
	      texture_format, GL_UNSIGNED_BYTE, surface->pixels );
	  } else {
		  printf("could not load texture: %s\n", filename.c_str());
	  }

	  if ( surface )
	    SDL_FreeSurface( surface);
	}





