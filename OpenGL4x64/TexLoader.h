
/*############################################################################
##	TexLoader																##
##																			##
##	Inline .h file															##
##	Input:	Texture Directory	Output:	??	 Array of Texture				##
##	Imports Texture Files in .bmp											##
##																			##
##	Author:		Justin Bentley										 ##  ## ##
##	Date:		03/2019												##  ##  ##
##	Version:	1.1												   ######   ##
##																# ##  ##    ##
#############################################################################*/

//textures need to be flipped upside down and saved before loading

//almost straight out of opengl 1.2 VIC

#ifndef TexLoader
#define TexLoader

#include <stdio.h>
#include <string.h>
#include "ThirdParty\Bitmap.h"

//#include "GL/glew.h"	//opengl handler

const int textures = 9;			//needed in the (self-contained) object
static GLuint tex[textures];	//

//Load Individual Textur; filename ..\Textures\****.bmp, TexHandle num, GL Tex Slot num, Clamp = 0 / Tile = 1
//will only take bitmap
inline void LoadTexture(
	char filename[60],	
	int TextureSlot,	
	int texhandlenum,	
	int ClampTile
	){

	char dir[70] = "../Textures\\";
	strcat(dir, filename);
	strcat(dir, ".bmp");

	GLuint glerror;

	CBitmap *bmp = new CBitmap(dir);

	//	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);//error
	//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//error

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);			//dont need to be called repeatadly
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);		//
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);		//
	glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);	//

	glActiveTexture(GL_TEXTURE0 + TextureSlot);//set active texture slot in gpu, must be called before setting tex

	glGenTextures(1, &tex[texhandlenum]);
	glBindTexture(GL_TEXTURE_2D, tex[texhandlenum]);

	glTexStorage2D(GL_TEXTURE_2D,
		9,
		GL_RGBA8,		//check
		bmp->GetWidth(),
		bmp->GetHeight()
	);
	glTexSubImage2D(GL_TEXTURE_2D,
		0,
		0,
		0,
		bmp->GetWidth(),
		bmp->GetHeight(),
		GL_RGBA,
		GL_UNSIGNED_BYTE,				// format for GL to store it
		bmp->GetBits()
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (ClampTile == 0) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	if (ClampTile == 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);//sharpest
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	//use me

	delete bmp;//	 <<dont forget

//	glBindTexture(GL_TEXTURE_2D, 0);//if unbind, big if as overwritten but oh well

}

inline void LoadAllTextures() {
	
	//note to self, u cant put functions in a header (like this :)), this part
	//will be in an object but above is planned to stay .h

	LoadTexture("BlackCamo", 0, 0, 0);
	LoadTexture("BlackCamo",1, 1, 0);
	LoadTexture("BlackCamo", 2, 2, 0);
	LoadTexture("BlackCamo", 3, 3, 0);
	LoadTexture("BlackCamo", 4, 4, 0);
	LoadTexture("BlackCamo", 5, 5, 0);
	LoadTexture("BlackCamo", 6, 6, 0);
	LoadTexture("BlackCamo", 7, 7, 0);
	LoadTexture("BlackCamo", 8, 8, 0);

	//LoadTexture("Mario01", 0, 0, 0);
	//LoadTexture("Mario01", 1, 1, 0);
	//LoadTexture("Mario01", 2, 2, 0);
	//LoadTexture("Mario01", 3, 3, 0);
	//LoadTexture("Mario01", 4, 4, 0);
	//LoadTexture("Mario01", 5, 5, 0);
	//LoadTexture("Mario01", 6, 6, 0);
	//LoadTexture("Mario01", 7, 7, 0);
	//LoadTexture("Mario01", 8, 8, 0);

	//LoadTexture("mossyTexture", 0, 0, 0);
	//LoadTexture("pavingTexture", 1, 1, 0);
	//LoadTexture("BlackCamo", 2, 2, 0);
	//LoadTexture("mossyTexture", 3, 3, 0);
	//LoadTexture("pavingTexture", 4, 4, 0);
	//LoadTexture("BlackCamo", 5, 5, 0);
	//LoadTexture("mossyTexture", 6, 6, 0);
	//LoadTexture("pavingTexture", 7, 7, 0);
	//LoadTexture("BlackCamo", 8, 8, 0);

	//LoadTexture("BlackCamo", 0, 0, 0);
	//LoadTexture("pavingTexture", 1, 1, 0);
	//LoadTexture("Smile", 2, 2, 0);
	//LoadTexture("RedCamo", 3, 3, 0);
	//LoadTexture("Tree256x256", 4, 4, 0);
	//LoadTexture("Mario01", 5, 5, 0);
	//LoadTexture("GOES12Fulldiskwatervapor", 6, 6, 0);
	//LoadTexture("WoodFloor01", 7, 7, 0);
	//LoadTexture("mossyTexture", 8, 8, 0);



}
inline void DestroyLoadTexture() {
	
	for (int i = 0; i < textures; i++) {
		glDeleteTextures(1, &tex[i]);//	<<dont forget
	}
}

#endif



/*
glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, Smile);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

//BI-LINIEAR
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//linear interp //bi-linear //average neigbour texels
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

*/