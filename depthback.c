#include <stdio.h>
#include <stdlib.h>


#include <GL/glew.h>
#include <GL/gl.h>

#include "globaldefs.h"
#include "shadermanager.h"
#include "matrixlib.h"
#include "camera.h"
#include "depthback.h"



GLuint depthwidth = 640;
GLuint depthheight = 480;
GLuint depthtexid = 0;
GLuint *depthdata;
GLuint numdepthverts;
GLuint depthvao;

shader_t depthbackshader;
int depthneedsupdate = FALSE;


void depth_render(camera_t *c){
	glBindTexture(GL_TEXTURE_2D, depthtexid);
	glUseProgram(depthbackshader.programid);
	glBindVertexArray(depthvao);
	GLfloat mvp[16];
	Matrix4x4_ToArrayFloatGL(&c->mvp, mvp);
	glUniformMatrix4fv(depthbackshader.unimat40, 1, GL_FALSE, mvp);
	glUniform3f(depthbackshader.univec30, 1.0/depthwidth, 1.0/depthwidth, 1.0);
	glUniform2i(depthbackshader.univec20, depthwidth, depthheight);
	glDrawArrays(GL_POINTS, 0, numdepthverts);
}
int createback(const int x, const int y, const float scalex, const float scaley){
	unsigned int fx = x-1;
	unsigned int fy = y-1;
	numdepthverts =  x *y ;
	GLfloat * wavebuffer = malloc(numdepthverts * 2 * sizeof(GLfloat));
	int ix;
	int iy;
	for(iy = 0; iy < y; iy++){
	for(ix = 0; ix < x; ix++){
		wavebuffer[((iy * x + ix) * 2) +0] = (float) ix;
		wavebuffer[((iy * x + ix) * 2) +1] = (float) iy;
	}}
	glGenVertexArrays(1, &depthvao);
	glBindVertexArray(depthvao);
	GLuint vertid;
	glGenBuffers(1, &vertid);
	glBindBuffer(GL_ARRAY_BUFFER, vertid);
	glBufferData(GL_ARRAY_BUFFER, numdepthverts * 2 * sizeof(GLfloat), wavebuffer, GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSATTRIBLOC);
	glVertexAttribPointer(POSATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*) 0);
	return TRUE;
}



void depth_update(void){
	if(!depthneedsupdate) return;
	glBindTexture(GL_TEXTURE_2D, depthtexid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, depthwidth, depthheight, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, depthdata);
	depthneedsupdate = FALSE;
}


void depth_init(void){

	depthbackshader = shader_load("depthback");
	createback(640, 480, 1.0, 1.0);

	depthneedsupdate = TRUE;
	depthdata = malloc(depthwidth * depthheight * sizeof(GLushort));
	glGenTextures(1, &depthtexid);
	depth_update();
}
