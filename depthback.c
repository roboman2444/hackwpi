#include <stdio.h>
#include <stdlib.h>


#include <GL/glew.h>
#include <GL/gl.h>
#include <pthread.h>

#include "globaldefs.h"
#include "shadermanager.h"
#include "matrixlib.h"
#include "camera.h"
#include "depthback.h"

#include "postprocess.h"

#include "glstates.h"
#include "freenect_sync/libfreenect_buffer.h"
#include "cubemap.h"

GLuint depthwidth = 640;
GLuint depthheight = 480;
GLuint depthtexid = 0;
GLuint coltexid = 0;
GLfloat *depthdata;
GLuint *videodata;
GLuint numdepthverts;
GLuint depthvao;

shader_t depthbackshader;


void depth_render(camera_t *c){
	states_bindActiveTexture(0, GL_TEXTURE_2D, depthtexid);
	states_bindActiveTexture(2, GL_TEXTURE_2D, coltexid);
	states_bindActiveTexture(1, GL_TEXTURE_CUBE_MAP, cubemapid);
	glUseProgram(depthbackshader.programid);
	glBindVertexArray(depthvao);
	//glBindVertexArray(fsquad);
	GLfloat mvp[16];
	Matrix4x4_ToArrayFloatGL(&c->mvp, mvp);
	glUniformMatrix4fv(depthbackshader.unimat40, 1, GL_FALSE, mvp);
	glUniform3f(depthbackshader.univec31, c->pos[0], c->pos[1], c->pos[2]);
	glUniform3f(depthbackshader.univec30, 10.0/depthwidth, 10.0/depthwidth, 1.0);
	glUniform2i(depthbackshader.univec20, depthwidth, depthheight);
	glDrawArrays(GL_POINTS, 0, numdepthverts);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
	pthread_mutex_lock(&depth_mutex);
	if (depth_data_ready) {
		int i;
		for(i = 0; i < depthwidth * depthheight; i++) {
			depthdata[i] = ((GLfloat) rawdepthdata[i])*-0.0001 + 1.0;
		}
	}
	depth_data_ready = FALSE;
	pthread_mutex_unlock(&depth_mutex);

	pthread_mutex_lock(&video_mutex);
	if (video_data_ready) {
		// videodata = rawvideodata;
		int i;
		for (i = 0; i < 640*480; i++) {
			videodata[i] = rawvideodata[i];
		}
	}
	video_data_ready = FALSE;
	pthread_mutex_unlock(&video_mutex);

	states_bindActiveTexture(0, GL_TEXTURE_2D, depthtexid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, depthwidth, depthheight, 0, GL_RED, GL_FLOAT, depthdata);
//	states_bindActiveTexture(0, GL_TEXTURE_2D, coltexid);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, depthwidth/2, depthheight/2, 0, GL_RGB, GL_UNSIGNED_BYTE, rawcolordata);
}

void depth_init(void){

	depthbackshader = shader_load("depthback");
//	depthbackshader = shader_load("sexturedmesh");
	createback(depthwidth, depthheight, 1.0, 1.0);

	depthdata = malloc(depthwidth * depthheight * sizeof(GLfloat));
	videodata = (uint8_t *) malloc(640 * 480 * 3);
	glGenTextures(1, &depthtexid);
	states_bindActiveTexture(0, GL_TEXTURE_2D, depthtexid);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenTextures(1, &coltexid);
	states_bindActiveTexture(0, GL_TEXTURE_2D, coltexid);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	depth_update();
}
