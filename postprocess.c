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

#include "glstates.h"
#include "postprocess.h"



shader_t bloomout;
shader_t dof;
shader_t fsblurh;
shader_t fsblurv;
shader_t lensflare;
shader_t lensout;

framebuffer_t fulls;
framebuffer_t fourths;
framebuffer_t eights;
GLuint fsquad;
struct texinfo lenscolor;
struct texinfo lensdirt;
struct texinfo lensstar;

void loadshaders(void){
	bloomout = shader_load("bloomout");
	lensout = shader_load("lensout");
	fsblurv = shader_load("fsblurv");
	fsblurh = shader_load("fsblurh");
	lensflare = shader_load("lensflare");



}
void loadtextures(void){
	tex_load("lenscolor.png", &lenscolor);
	tex_load("lensdirt.png", &lensdirt);
	tex_load("lensstar.png", &lensstar);
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


GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
GLenum fullsint[] = {GL_RGBA16F, GL_RGB16F, 0, 0, 0, 0, 0, 0};
GLenum fullsfmt[] = {GL_RGBA, GL_RGB, 0, 0, 0, 0, 0, 0};
GLenum fullstype[] = {GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, 0, 0, 0, 0, 0, 0};

GLenum fourthsint[] = {GL_RGB16F, GL_RGB16F, GL_RGB16F, 0, 0, 0, 0, 0};
GLenum fourthsfmt[] = {GL_RGB, GL_RGB, GL_RGB, 0, 0, 0, 0, 0};
GLenum fourthstype[] = {GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, 0, 0, 0, 0, 0};

GLenum eisint[] = {GL_RGB16F, GL_RGB16F, 0, 0, 0, 0, 0, 0};
GLenum eisfmt[] = {GL_RGB, GL_RGB, 0, 0, 0, 0, 0, 0};
GLenum eistype[] = {GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, 0, 0, 0, 0, 0, 0};

void genframes(int width, int height){
	fulls = create_framebuffer(width, height, fullsint, fullsfmt, fullstype, 1);
	fourths = create_framebuffer(width/4, height/4, fourthsint, fourthsfmt, fourthstype, 0);
	eights = create_framebuffer(width/8, height/8, eisint, eisfmt, eistype, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void post_init(int width, int height){
	createFSquad();
	genframes(width, height);
	loadshaders();
	loadtextures();
}
void bind_fs(void){
	glBindFramebuffer(GL_FRAMEBUFFER, fulls.framebuffer_id);
	glViewport(0,0, fulls.width, fulls.height);
	glDrawBuffers(1, buffers);
}

void runpost(void){
	glBindVertexArray(fsquad);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, fourths.framebuffer_id);
	glViewport(0,0, fourths.width, fourths.height);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glDrawBuffers(3, buffers);
	glDrawBuffers(1, buffers);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(fsblurv.programid);
	states_bindActiveTexture(0, GL_TEXTURE_2D, fulls.texture_id[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	glDrawBuffers(1, buffers+1);
//	glDrawBuffers(3, buffers);
	glUseProgram(fsblurh.programid);
	states_bindActiveTexture(0, GL_TEXTURE_2D, fourths.texture_id[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glBindFramebuffer(GL_FRAMEBUFFER, fulls.framebuffer_id);
	glViewport(0,0, fulls.width, fulls.height);
//	glDrawBuffers(1, buffers+1);
	glDrawBuffers(1, buffers);
//	glDrawBuffers(3, buffers);
	glUseProgram(bloomout.programid);
	states_bindActiveTexture(0, GL_TEXTURE_2D, fulls.texture_id[0]);
	states_bindActiveTexture(1, GL_TEXTURE_2D, fourths.texture_id[1]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
/*

	glBindFramebuffer(GL_FRAMEBUFFER, eights.framebuffer_id);
	glViewport(0,0, eights.width, eights.height);
	glDrawBuffers(3, buffers);
//	glDrawBuffers(1, buffers+2);
	glUseProgram(lensflare.programid);
	states_bindActiveTexture(0, GL_TEXTURE_2D, fulls.texture_id[1]);
	states_bindActiveTexture(1, GL_TEXTURE_2D, lenscolor.texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	glDrawBuffers(3, buffers);
//	glDrawBuffers(1, buffers);
	glUseProgram(fsblurv.programid);
	states_bindActiveTexture(0, GL_TEXTURE_2D, eights.texture_id[2]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

//	glDrawBuffers(1, buffers+1);
	glDrawBuffers(3, buffers);
	glUseProgram(fsblurh.programid);
	states_bindActiveTexture(0, GL_TEXTURE_2D, eights.texture_id[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0, fulls.width, fulls.height);
//	glDrawBuffers(1, buffers);
	glDrawBuffers(3, buffers);
	glUseProgram(lensout.programid);
	states_bindActiveTexture(0, GL_TEXTURE_2D, fulls.texture_id[1]);
	states_bindActiveTexture(1, GL_TEXTURE_2D, eights.texture_id[1]);
	states_bindActiveTexture(2, GL_TEXTURE_2D, lensdirt.texture);
	states_bindActiveTexture(3, GL_TEXTURE_2D, lensstar.texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
*/

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
}
