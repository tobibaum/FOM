/*
 * TextureManager.h
 *
 *  Created on: Jun 6, 2011
 *      Author: Tobi
 */

#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include "GlutStuff.h"
#include <map>
#include <stdio.h>
#include <string>

typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef long 				LONG;
typedef unsigned char 		BYTE;

//File information header
//provides general information about the file
typedef struct tagBITMAPFILEHEADER {
  WORD    bfType;
  DWORD   bfSize;
  WORD    bfReserved1;
  WORD    bfReserved2;
  DWORD   bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

//Bitmap information header
//provides information specific to the image data
typedef struct tagBITMAPINFOHEADER{
  DWORD  biSize;
  LONG   biWidth;
  LONG   biHeight;
  WORD   biPlanes;
  WORD   biBitCount;
  DWORD  biCompression;
  DWORD  biSizeImage;
  LONG   biXPelsPerMeter;
  LONG   biYPelsPerMeter;
  DWORD  biClrUsed;
  DWORD  biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

//Colour palette
typedef struct tagRGBQUAD {
  BYTE    rgbBlue;
  BYTE    rgbGreen;
  BYTE    rgbRed;
  BYTE    rgbReserved;
} RGBQUAD;



class TextureManager {
public:
	TextureManager();
	virtual ~TextureManager();

	static unsigned int getTextureHandle(std::string filename);



private:
	static void loadTexture(std::string filename);

	static std::map<std::string, unsigned int> m_texturehandles;
};

#endif /* TEXTUREMANAGER_H_ */
