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
GLuint numdepthfaces;
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
	glDrawElements(GL_TRIANGLES, 3 * numdepthfaces, GL_UNSIGNED_INT, 0);
}
int createback(const int x, const int y, const float scalex, const float scaley){
	unsigned int fx = x;
	unsigned int fy = y;
	unsigned int maxx = x+1;
	unsigned int maxy = y+1;
	numdepthfaces = 10 * fx * fy;
	numdepthverts = 8 * fx * fy;
	GLfloat * wavebuffer = malloc(numdepthverts * 5 * sizeof(GLfloat));
	GLuint *waveindice = malloc(numdepthfaces * 3 * sizeof(GLuint));
	unsigned int i;
	for(i = 0; i < fx * fy; i++){
		//top face
		waveindice[i * 30+0] = (i * 8)+0;
		waveindice[i * 30+1] = (i * 8)+1;
		waveindice[i * 30+2] = (i * 8)+2;

		waveindice[i * 30+3] = (i * 8)+0;
		waveindice[i * 30+4] = (i * 8)+2;
		waveindice[i * 30+5] = (i * 8)+3;
		//back face
		waveindice[i * 30+6] = (i * 8)+4;
		waveindice[i * 30+7] = (i * 8)+5;
		waveindice[i * 30+8] = (i * 8)+1;

		waveindice[i * 30+9]  = (i * 8)+4;
		waveindice[i * 30+10] = (i * 8)+1;
		waveindice[i * 30+11] = (i * 8)+0;
		//front face
		waveindice[i * 30+12] = (i * 8)+6;
		waveindice[i * 30+13] = (i * 8)+7;
		waveindice[i * 30+14] = (i * 8)+3;

		waveindice[i * 30+15] = (i * 8)+6;
		waveindice[i * 30+16] = (i * 8)+3;
		waveindice[i * 30+17] = (i * 8)+2;
		//left face
		waveindice[i * 30+18] = (i * 8)+7;
		waveindice[i * 30+19] = (i * 8)+4;
		waveindice[i * 30+20] = (i * 8)+0;

		waveindice[i * 30+21] = (i * 8)+7;
		waveindice[i * 30+22] = (i * 8)+0;
		waveindice[i * 30+23] = (i * 8)+3;
		//right face
		waveindice[i * 30+24] = (i * 8)+5;
		waveindice[i * 30+25] = (i * 8)+6;
		waveindice[i * 30+26] = (i * 8)+2;

		waveindice[i * 30+27] = (i * 8)+5;
		waveindice[i * 30+28] = (i * 8)+2;
		waveindice[i * 30+29] = (i * 8)+1;
	}


	unsigned int mx, my;
	i=0;
	for(my = 0; my < fy; my++){
		unsigned int plusy = my+1;
		float yl = (scaley / (float)maxy) * my;
		float yp = (scaley / (float)maxy) * plusy;
		float ytc = my /(float)fy;
		for(mx = 0; mx < fx; mx++){
			unsigned int plusx = mx+1;
			float xl = (scalex / (float)maxx) * mx;
			float xp = (scalex / (float)maxx) * plusx;
			float xtc = mx /(float)fx;
				wavebuffer[40 * x * y + 0] = xl;
				wavebuffer[40 * x * y + 1] = yl;
				wavebuffer[40 * x * y + 2] = 1.0;
				wavebuffer[40 * x * y + 3] = xtc;
				wavebuffer[40 * x * y + 4] = ytc;

				wavebuffer[40 * x * y + 5] = xp;
				wavebuffer[40 * x * y + 6] = yl;
				wavebuffer[40 * x * y + 7] = 1.0;
				wavebuffer[40 * x * y + 8] = xtc;
				wavebuffer[40 * x * y + 9] = ytc;

				wavebuffer[40 * x * y + 10] = xp;
				wavebuffer[40 * x * y + 11] = yp;
				wavebuffer[40 * x * y + 12] = 1.0;
				wavebuffer[40 * x * y + 13] = xtc;
				wavebuffer[40 * x * y + 14] = ytc;

				wavebuffer[40 * x * y + 15] = xl;
				wavebuffer[40 * x * y + 16] = yp;
				wavebuffer[40 * x * y + 17] = 1.0;
				wavebuffer[40 * x * y + 18] = xtc;
				wavebuffer[40 * x * y + 19] = ytc;

				wavebuffer[40 * x * y + 20] = xl;
				wavebuffer[40 * x * y + 21] = yl;
				wavebuffer[40 * x * y + 22] = 0.0;
				wavebuffer[40 * x * y + 23] = xtc;
				wavebuffer[40 * x * y + 24] = ytc;

				wavebuffer[40 * x * y + 25] = xp;
				wavebuffer[40 * x * y + 26] = yl;
				wavebuffer[40 * x * y + 27] = 0.0;
				wavebuffer[40 * x * y + 28] = xtc;
				wavebuffer[40 * x * y + 29] = ytc;

				wavebuffer[40 * x * y + 30] = xp;
				wavebuffer[40 * x * y + 31] = yp;
				wavebuffer[40 * x * y + 32] = 0.0;
				wavebuffer[40 * x * y + 33] = xtc;
				wavebuffer[40 * x * y + 34] = ytc;

				wavebuffer[40 * x * y + 35] = xl;
				wavebuffer[40 * x * y + 36] = yp;
				wavebuffer[40 * x * y + 37] = 0.0;
				wavebuffer[40 * x * y + 38] = xtc;
				wavebuffer[40 * x * y + 39] = ytc;
		}
	}
	glGenVertexArrays(1, &depthvao);
	glBindVertexArray(depthvao);
	GLuint vertid;
	GLuint indid;
	glGenBuffers(1, &vertid);
	glGenBuffers(1, &indid);
	glBindBuffer(GL_ARRAY_BUFFER, vertid);
	glBufferData(GL_ARRAY_BUFFER, numdepthverts * 5 * sizeof(GLfloat), wavebuffer, GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSATTRIBLOC);
	glVertexAttribPointer(POSATTRIBLOC, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) 0);
	glEnableVertexAttribArray(TCATTRIBLOC);
	glVertexAttribPointer(TCATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
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
