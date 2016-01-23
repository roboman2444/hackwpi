#include <stdio.h>
#include <stdlib.h>


#include <GL/glew.h>
#include <GL/gl.h>

#include "globaldefs.h"
#include "depthback.h"



GLuint depthwidth = 640;
GLuint depthheight = 480;
GLuint depthtexid = 0;
GLuint *depthdata;
int depthneedsupdate = FALSE;


void depth_update(void){
	if(!depthneedsupdate) return;
	glBindTexture(GL_TEXTURE_2D, depthtexid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, depthwidth, depthheight, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, depthdata);
	depthneedsupdate = FALSE;
}

void depth_init(void){
	depthneedsupdate = TRUE;
	depthdata = malloc(depthwidth * depthheight * sizeof(GLushort));
	glGenTextures(1, &depthtexid);
	depth_update();
}
