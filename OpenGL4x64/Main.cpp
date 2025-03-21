/*############################################################################
##                                                                          ##
##  OpenGL4x64                                                              ##
##                                                                          ##
##  Author:		Justin Bentley                        #####  ######  ###### ##
##  Date:		21/03/2019                          ##      ##  ##  ##  ##  ##
##  Version:    1.0                                ##      ######  ######   ##
##                                             ##  #####  ##      ##        ##
##############################################################################


OPENGL 3-4 (MODERN)
DEPENDANT ON SHADER VERSION

(x64) ONLY (SEE FOOTER)
USING GLFW, GLEW

THIS FILE MAINLY FOR WINDOW & RENDERING SETUP
& LINKING TO KEYBOARD ETC (IN THOERY)

INTERMEDIATE \x64\ DIRECTORY BETWEEN SOLUTION AND EXE REMOVED (SEE FOOTER)

*/

#pragma warning(disable : 4996) // Reduce unsafe warning messages

#include <stdlib.h>
#include <stdio.h>//console out
//#include <gl\GL.h>

//#define ASSERT(x) if (!(x)) __debugbreak(); //if (x) not met Visual Studio (Specificly) will breakpoint
//use ASSERT(!glerror); or vice versa

//#define FLAG_RECORD_MOVEMENTS

//*ThirdParties
#define GLEW_STATIC		//define using staticaly linked lib glew32s.lib
#include "GL/glew.h"	//opengl handler
#include "GLFW/glfw3.h"	//context handler
//#include "../ThirdParties/glm/glm.hpp"	//would rather datenwulf's linmath.h TBH
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"


//#include <iostream>
//#include "linmath.h"

//#include <cmath>

#include <time.h>

//*Program Core
//#include "Inline14Tester.h"
#include "Shaders.h"
#include "TexLoader.h"
#include "Trig.h"

//GL_INVALID_ENUM 0x0500		1280
//GL_INVALID_VALUE 0x0501		1281
//GL_INVALID_OPERATION 0x0502	1282

int main(int argc, char * argv[])
{
	srand(time(NULL));

	GLuint glerror;
	GLFWwindow* window;
	//	int DeviceWidth = 1080;	//htc vive res, poll openvr
	//	int DeviceHeight = 1200; //needs to be polled by openvr

		/* Initialize the library */
	if (!glfwInit()) { printf("glfw load problem"); getchar(); return -1; }



	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//planning to target 4.3 nvid 760 support ? (see shaders)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//core or compatability (COMPAT)


	/* Create a windowed mode window and its OpenGL context */
	glfwWindowHint(GLFW_AUTO_ICONIFY, 0);//dont minimise on focus loss
	//	glfwWindowHint(GLFW_DECORATED, 0);//hide border, minimise and terminate buttons
	//	glfwWindowHint(GL_STEREO, 1);//steroscopic, maybe nvidia tv


	//windowing
	int monitorcount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorcount);  // (make your functions like this)
	printf("Number of Monitors: %d\n", monitorcount);
	//if (monitorcount > 1) { *monitors++; }//if secondary monitor, use it, for now
//	window = glfwCreateWindow(1920, 1080, "Hello World", *monitors, NULL);


	window = glfwCreateWindow(1880, 1040, "OpenGL4x64", NULL, NULL);

	//	if (!window) { glfwTerminate(); return -1; }

		/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//	GLuint i = glewInit(); printf("%i", i);//bp will give error

		/*put glew init here*/
	if (glewInit() != GLEW_OK) { printf("glew load problem"); getchar(); return -1; }

//	printf("GL VERSION CHECK: %s\n", glGetString(GL_VERSION));



	printf("%s \n", glGetString(GL_VERSION));
	int MaxTextureImageUnits; glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MaxTextureImageUnits);
	printf("Max Texture Slots: %d \n", MaxTextureImageUnits);


	/*Vertex Buffer*/
	const int trielements = 12;
	GLfloat triangleverts[] = {
		//	x		y		s(u)	t(v)
			-0.5f,	-0.5f,	0.0f,	0.0f,	//index 0
			0.5f,	-0.5f,	1.0f,	0.0f,	//index 1
			0.5f,	0.5f,	1.0f,	1.0f,	//index 2
			-0.5f,	0.5f,	0.0f,	1.0f	//index 3
	};
	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	//vertex array object, contains vert buffer, not index
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);//set active in gl struct

	//*vertex buffer*, holds vertex's, normals, tex co-ords etc
	unsigned int vertbuffer;//will be the buffer id
	glGenBuffers(1, &vertbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertbuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), triangleverts, GL_STATIC_DRAW);

	unsigned int offset = (sizeof(GLfloat) * 2);

	//lynda declares this after element array buffer
	//vertex attrib pointer buffer, labels verts, tex, normals
	glEnableVertexAttribArray(0); //after attribpointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);//2nd last stride

	glerror = glGetError(); if (glerror > 0) { printf("GL CALL ERROR: %i", glerror); }

	glEnableVertexAttribArray(1); //after attribpointer
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid *)(sizeof(GLfloat) * 2));//2nd last stride

	glerror = glGetError(); if (glerror > 0) { printf("GL CALL ERROR: %i", glerror); }

	//index buffer, like faces in .obj, which vert in vertbuffer to draw
	unsigned int indexbuffer;//will be the buffer id, aka ibo (index buffer object)
	glGenBuffers(1, &indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//	glm::mat4 projectionmatrix = glm::ortho(-1.0f,1.0f,-1.5f,1.5f,-1.0f,1.0f);
	glm::mat4 projectionmatrix;//camera lens, projection matrix, need an aspect ratio function here
	glm::mat4 modelviewmatrix;// = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0, 0));//translate modelview matrix //start pos

	glm::mat4 globalorigin;

	const int g3 = 99;
	int array[g3];//	<<

	glerror = glGetError(); if (glerror > 0) { printf("GL CALL ERROR: %i", glerror); __debugbreak(); }


	//create and set shaders actve from Shaders.h
	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);
	//get variable in shader
	int uniformcolor = glGetUniformLocation(shader, "u_Color");//uniformcolor = id	//rename s_location
	int uniformtexture = glGetUniformLocation(shader, "u_Texture");//will need
	int uniformprojection = glGetUniformLocation(shader, "u_MVP");//


	glerror = glGetError(); if (glerror > 0) { printf("GL CALL ERROR: %i", glerror); __debugbreak(); }


	//assign uniformtexture
	LoadAllTextures();

	//debind everything, dunno why //update: unbinding not required
	//unbind vao first
	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//rebind everything
	glUseProgram(shader);//set shader as the active active
	glBindVertexArray(vao);//set active in gl struct
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);

	/*Getting .h'ed*/

//#define BUBBLES_SYNC_COLOR	//declare only one
//#define BUBBLES_UNSYNC_COLOR
#define BUBBLES_STAGGER_COLOR
#ifdef BUBBLES_SYNC_COLOR
	GLfloat rgbval[9][3] = {{ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }};
	int rgorb[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };	//rg or b
	GLfloat mag = 0.01f;	//nitude
	bool updown[9] = { true, true, true, true, true, true, true, true, true };
#endif
#ifdef BUBBLES_UNSYNC_COLOR
	GLfloat rgbval[9][3] = { { 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f } };
	int rgorb[9] = { 0, 1, 2, 0, 1, 2, 0, 1, 2 };	//rg or b
	GLfloat mag = 0.01f;	//nitude
	bool updown[9] = { true, true, true, true, true, true, true, true, true };
#endif
#ifdef BUBBLES_STAGGER_COLOR
	GLfloat rgbval[9][3] = { { 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.6f },{ 0.0f,0.4f,0.0f },
	{ 0.0f,0.9f,0.0f },{ 0.0f,0.0f,0.2f },{ 0.8f,0.0f,0.0f },{ 0.0f,0.5f,0.0f },
	{ 0.2f,0.0f,0.0f },{ 0.0f,0.0f,1.0f } };
	int rgorb[9] = { 0, 2, 1, 1, 2, 0, 1, 0, 2 };	//rg or b
	GLfloat mag = 0.01f;	//nitude
	bool updown[9] = { true, true, true, false, true, false, true, true, false };
#endif

	bool equals = true;
	if (equals) { printf("hello"); }
	GLfloat magl = 0.008f;	//nitude
	GLfloat fac = 5;
	bool rlupdown[9] = {true, true, true, true, true, true, true, true, true };
	bool tbupdown[9] = {false, false, false, false, false, false, false, false, false };
	GLfloat
		lon[9] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },//londitude (vertical)
		lat[9] = { 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f };
	int bubbles_num = 9;//number of bubbles	//max 9
	float bubble_boundary = 0.15f;
	int bubble_invincability = 140 * bubbles_num;

	int jne = bubbles_num;

//	projectionmatrix = glm::ortho(lon*-1, lon, lat*-1, lat, -1.0f, 1.0f);//u do not want this in ur draw call	//only update on resize event (if any)

	projectionmatrix = glm::ortho(-2.0f,2.0f,-1.5f,1.5f,-1.0f,1.0f);//ortho may imply 0 fov angle


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{	glClear(GL_COLOR_BUFFER_BIT);
		for (int i = 0; i < bubbles_num; i++) {
			//color flasher
			if (updown[i]) { rgbval[i][rgorb[i]] += mag; if (rgbval[i][rgorb[i]] > 1.0f) { updown[i] = !updown[i]; } }
			else { rgbval[i][rgorb[i]] -= mag; if (rgbval[i][rgorb[i]] < 0.30f) {
					rgorb[i]++; if (rgorb[i] > 2) { rgorb[i] = 0; } updown[i] = !updown[i]; }}
			//lon lat stretcher
			if (rlupdown[i]) { lon[i] -= magl + (i * magl / fac); if (lon[i] < -1.0f) { rlupdown[i] = !rlupdown[i]; } }
			else { lon[i] += magl + (i * magl / fac);  if (lon[i] > 1.0f) { rlupdown[i] = !rlupdown[i]; } }
			if (tbupdown[i]) { lat[i] -= magl + (i * magl / fac); if (lat[i] < -1.5f) { tbupdown[i] = !tbupdown[i]; } }
			else { lat[i] += magl + 0.003f + (i * magl / fac);  if (lat[i] > 1.5f) { tbupdown[i] = !tbupdown[i]; } }
			//bubble anti collide cooldown
			if (bubble_invincability > 0) { bubble_invincability--; if (bubble_invincability != 0) { goto bubble_invincable; }}
			//buuble collide check
			for (int j = 0; j < bubbles_num; j++) {
				for (int k = 0; k < bubbles_num; k++) {
					if ((DistSimple(lat[j], lon[j], 0, lat[k], lon[k], 0, bubble_boundary)) && (j != k)) {
						rlupdown[j] = !rlupdown[j]; tbupdown[j] = !tbupdown[j];
						//lat[j] += (rand() % 10) / 50; lon[j] += (rand() % 10) / 50;
						if (lat[j] < lat[k]) { lat[j] -= magl; lat[k] += magl; }
						else { lat[j] += magl; lat[k] -= magl; }}}} bubble_invincable:
			modelviewmatrix = glm::translate(glm::mat4(1.0f), glm::vec3(lat[i], lon[i], 0));
			globalorigin = projectionmatrix * modelviewmatrix;//not sure to multithread or gpu shade ??
			//shader uniforms, handles to the shader
			glUniform4f(uniformcolor, rgbval[i][0], rgbval[i][1], rgbval[i][2], 1.0f);//fragment shader color input
			glUniform1i(uniformtexture, i);//handle, gl texture slot
			glUniformMatrix4fv(uniformprojection, 1, GL_FALSE, &globalorigin[0][0]);//handle, num matx's, transpose, *projmatrix

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);//draw vert buffer using indexbuffer

		}

		//je++; if (je != bubbles_num) { goto jumpequal; }

		glerror = glGetError(); if (glerror > 0) { printf("GL CALL ERROR: %i", glerror); __debugbreak(); }


//		glDrawArrays(GL_TRIANGLES, 0, trielements / 2);//draw currently bound vert buffer w/o indexbuffer

		//OpenGL Specific Syntax Errors
		glerror = glGetError(); if (glerror > 0) { printf("GL CALL ERROR: %i", glerror); __debugbreak(); }

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
		
	}


	exit:

	DestroyLoadTexture();//glDeleteTextures
	glDeleteProgram(shader);//gpu leak :)	//Shaders.h
	
	glfwTerminate();
	return 0;
}



/*
INTERMEDIATE \x64\ DIRECTORY BETWEEN SOLUTION AND EXE REMOVED;

if x86 exe will be in $SolutionDir\Release\exe
if x64 exe will be in $SolutionDir\x64\Release\exe
for certain reasons this intermediate directory (folder) is annoying, 
specifically when syncing VS inbuilt exe and standalone exe by using 
..\ (back one dir) for say ..\MeshData

Select Properties for project file in solution explorer, OpenGL4x64 in
this case Select Configuration = All Configurations
Clone Win32 Output & Intermediate Directory to x64 Output & Intermediate
Directory

Now you can use; 
Load("../MeshData//InnerBlueSphere.obj"); throughout VS & standalone x64 exe's

**See ChernoProject OpenGL Video 2

LINKING;

C/C++/GENERAL/ADDITIONAL INC DIR/$(SolutionDir)Release\Thirdparties
C/C++/PREPROCESSOR/PREPROCESOR DEF/GLEW_STATIC(moved to heading Main.cpp)
LINKER/GENERAL/ADDITIONAL LIBRARY DIR/$(SolutionDir)Release\Thirdparties
LINKER/INPUT/ADDITIONAL DEPENDANCIES/glfw3.lib; glew32s.lib








//nice but not needed, use to automatically breakpoint
#define ASSERT(x) if (!(x)) __debugbreak(); //if (x) not met Visual Studio (Specificly) will breakpoint

//armygreen = 0.08, 0.3, 0











*/