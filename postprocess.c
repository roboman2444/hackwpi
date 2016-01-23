#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>


#include "globaldefs.h"
#include "shadermanager.h"
#include "matrixlib.h"
#include "framebuffers.h"
#include "tex.h"
#include "glhelp.h"

#include "postprocess.h"


shader_t fsquadshader;
shader_t bloomoutshader;
shader_t dofshader;
shader_t fsblurh;
shader_t fsblurv;
shader_t lensflare;
shader_t lenscombine;
GLuint fsquad;
Framebuffer screen;
Framebuffer bloomv;
Framebuffer bloomh;
Framebuffer bloomout;
Framebuffer fpscreen;
Framebuffer lensout;
Framebuffer lensoutblurv;
Framebuffer lensoutblurh;
struct texinfo lenscolor;
struct texinfo lensdirt;
struct texinfo lensstar;

void loadshaders(void){
	bloomoutshader = shader_load("bloomout");
//	lensoutshader = shader_load("lensout");
	fsblurv = shader_load("fsblurv");
	fsblurh = shader_load("fsblurh");
//	lensoutblurv = shader_load("lensoutblurv");
//	lensoutblurh = shader_load("lensoutblurh");
	lensflare = shader_load("lensflare");
	fsblurv = shader_load("fsblurv");



}
void loadtextures(void){
	tex_load("lenscolor.png", &lenscolor);
	tex_load("lensdirt.png", &lensdirt);
	tex_load("lensstart.png", &lensstar);
}

void createFSquad(void){
	GLfloat fsquadv[] = {-1.0, -1.0, 0.0, 0.0, -1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 0.0};
	GLuint fsquadf[] = {0, 2, 1, 0, 3, 2};
	GLuint bid;
	GLuint iid;
	glGenVertexArrays(1, &fsquad);
	glBindVertexArray(fsquad);
	glGenBuffers(1, &bid);
	glGenBuffers(1, &iid);
	glBindBuffer(GL_ARRAY_BUFFER, bid);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 *sizeof(GLfloat), fsquadv, GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSATTRIBLOC);
	glVertexAttribPointer(POSATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) 0);
	glEnableVertexAttribArray(TCATTRIBLOC);
	glVertexAttribPointer(TCATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) (2 * sizeof(GLfloat)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), fsquadf, GL_STATIC_DRAW);
}


GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
void genframes(int width, int height){
	CHECKGLERROR;
	glGenFramebuffers(1, &fpscreen.framebuffer_id);
	fpscreen.width = width;
	fpscreen.height = height;
	glBindFramebuffer(GL_FRAMEBUFFER, fpscreen.framebuffer_id);
	CHECKGLERROR;
	glGenRenderbuffers(1, &fpscreen.renderbuffer_id);
	CHECKGLERROR;
	glBindRenderbuffer(GL_RENDERBUFFER, fpscreen.renderbuffer_id);
	CHECKGLERROR;
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fpscreen.width, fpscreen.height);
	CHECKGLERROR;
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fpscreen.renderbuffer_id);
	CHECKGLERROR;
	glGenTextures(1, &fpscreen.texture_id);
	glBindTexture(GL_TEXTURE_2D,fpscreen.texture_id);
	CHECKGLERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	CHECKGLERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CHECKGLERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	CHECKGLERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	CHECKGLERROR;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, fpscreen.width, fpscreen.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	CHECKGLERROR;
	glDrawBuffers(1, buffers);
	CHECKGLERROR;
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fpscreen.texture_id, 0);
	CHECKGLERROR;

	glGenFramebuffers(1, &lensout.framebuffer_id);
	lensout.width = width/8;
	lensout.height = height/8;
	glBindFramebuffer(GL_FRAMEBUFFER, lensout.framebuffer_id);
//	glGenRenderbuffers(1, &lensout.renderbuffer_id);
//	glBindRenderbuffer(GL_RENDERBUFFER, lensout.renderbuffer_id);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, lensout.width, lensout.height);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, lensout.renderbuffer_id);
	glGenTextures(1, &lensout.texture_id);
	glBindTexture(GL_TEXTURE_2D,lensout.texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, lensout.width, lensout.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glDrawBuffers(1, buffers);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lensout.texture_id, 0);


	glGenFramebuffers(1, &bloomout.framebuffer_id);
	bloomout.width = width;
	bloomout.height = height;
//	glBindFramebuffer(GL_FRAMEBUFFER, bloomout.framebuffer_id);
//	glGenRenderbuffers(1, &bloomout.renderbuffer_id);
//	glBindRenderbuffer(GL_RENDERBUFFER, bloomout.renderbuffer_id);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bloomout.width, bloomout.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, bloomout.renderbuffer_id);
	glGenTextures(1, &bloomout.texture_id);
	glBindTexture(GL_TEXTURE_2D,bloomout.texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, bloomout.width, bloomout.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glDrawBuffers(1, buffers);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomout.texture_id, 0);


	glGenFramebuffers(1, &bloomv.framebuffer_id);
	bloomv.width = width/4;
	bloomv.height = height/4;
//	bloomv.width = width;
//	bloomv.height = height;
	glBindFramebuffer(GL_FRAMEBUFFER, bloomv.framebuffer_id);
	CHECKGLERROR;
//	glGenRenderbuffers(1, &bloomv.renderbuffer_id);
//	glBindRenderbuffer(GL_RENDERBUFFER, bloomv.renderbuffer_id);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bloomv.width, bloomv.height);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, bloomv.renderbuffer_id);
	glGenTextures(1, &bloomv.texture_id);
	CHECKGLERROR;
	glBindTexture(GL_TEXTURE_2D, bloomv.texture_id);
	CHECKGLERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	CHECKGLERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CHECKGLERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	CHECKGLERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	CHECKGLERROR;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, bloomv.width, bloomv.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	CHECKGLERROR;
	glDrawBuffers(1, buffers);
	CHECKGLERROR;
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomv.texture_id, 0);
	CHECKGLERROR;
	printf("framebuffer  bloomv %i has texture %i\n", bloomv.framebuffer_id, bloomv.texture_id);


	glGenFramebuffers(1, &bloomh.framebuffer_id);
	bloomh.width = width/4;
	bloomh.height = height/4;
//	bloomh.width = width;
//	bloomh.height = height;
	glBindFramebuffer(GL_FRAMEBUFFER, bloomh.framebuffer_id);
//	glGenRenderbuffers(1, &bloomh.renderbuffer_id);
//	glBindRenderbuffer(GL_RENDERBUFFER, bloomh.renderbuffer_id);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bloomh.width, bloomh.height);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, bloomh.renderbuffer_id);
	glGenTextures(1, &bloomh.texture_id);
	glBindTexture(GL_TEXTURE_2D,bloomh.texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, bloomh.width, bloomh.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glDrawBuffers(1, buffers);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomh.texture_id, 0);
	printf("framebuffer  bloomh %i has texture %i\n", bloomh.framebuffer_id, bloomh.texture_id);



	glGenFramebuffers(1, &lensoutblurh.framebuffer_id);
	lensoutblurh.width = width/8;
	lensoutblurh.height = height/8;
	glBindFramebuffer(GL_FRAMEBUFFER, lensoutblurh.framebuffer_id);
//	glGenRenderbuffers(1, &lensoutblurh.renderbuffer_id);
//	glBindRenderbuffer(GL_RENDERBUFFER, lensoutblurh.renderbuffer_id);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, lensoutblurh.width, lensoutblurh.height);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, lensoutblurh.renderbuffer_id);
	glGenTextures(1, &lensoutblurh.texture_id);
	glBindTexture(GL_TEXTURE_2D,lensoutblurh.texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, lensoutblurh.width, lensoutblurh.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glDrawBuffers(1, buffers);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lensoutblurh.texture_id, 0);


	glGenFramebuffers(1, &lensoutblurv.framebuffer_id);
	lensoutblurv.width = width/8;
	lensoutblurv.height = height/8;
	glBindFramebuffer(GL_FRAMEBUFFER, lensoutblurv.framebuffer_id);
//	glGenRenderbuffers(1, &lensoutblurv.renderbuffer_id);
//	glBindRenderbuffer(GL_RENDERBUFFER, lensoutblurv.renderbuffer_id);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, lensoutblurv.width, lensoutblurv.height);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, lensoutblurv.renderbuffer_id);
	glGenTextures(1, &lensoutblurv.texture_id);
	glBindTexture(GL_TEXTURE_2D,lensoutblurv.texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, lensoutblurv.width, lensoutblurv.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glDrawBuffers(1, buffers);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lensoutblurv.texture_id, 0);



}


void post_init(int width, int height){
	createFSquad();
	genframes(width, height);
	loadshaders();
	loadtextures();
}
void bind_fs(void){
	glBindFramebuffer(GL_FRAMEBUFFER, fpscreen.framebuffer_id);
}

void runpost(void){
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(fsquad);
	glBindFramebuffer(GL_FRAMEBUFFER, bloomv.framebuffer_id);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glClear(GL_COLOR_BUFFER_BIT); //may be able to remove
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fpscreen.texture_id);
	glUseProgram(fsblurv.programid);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

//	glBindFramebuffer(GL_FRAMEBUFFER, bloomh.framebuffer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glClear(GL_COLOR_BUFFER_BIT); //may be able to remove
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bloomv.texture_id);
	glUseProgram(fsblurh.programid);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, bloomout.framebuffer_id);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glClear(GL_COLOR_BUFFER_BIT); //may be able to remove
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bloomh.texture_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fpscreen.texture_id);
	glUseProgram(bloomoutshader.programid);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
/*

	glBindFramebuffer(GL_FRAMEBUFFER, lensout.framebuffer_id);
	glClear(GL_COLOR_BUFFER_BIT); //may be able to remove
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, lenscolor.texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bloomout.texture_id);
//	glActiveTexture(GL_TEXTURE2);
//	glBindTexture(GL_TEXTURE_2D, fpscreen.texture_id);
	glUseProgram(lensflare.programid);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, lensoutblurv.framebuffer_id);
	glClear(GL_COLOR_BUFFER_BIT); //may be able to remove
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lensout.texture_id);
	glUseProgram(fsblurv.programid);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	glBindFramebuffer(GL_FRAMEBUFFER, lensoutblurh.framebuffer_id);
	glClear(GL_COLOR_BUFFER_BIT); //may be able to remove
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lensoutblurv.texture_id);
	glUseProgram(fsblurh.programid);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT); //may be able to remove
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, lensdirt.texture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, lensstar.texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, lensoutblurh.texture_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bloomout.texture_id);

	glUseProgram(lenscombine.programid);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
*/

	glEnable(GL_DEPTH_TEST);

}
