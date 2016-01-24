#include <stdio.h>
#include <stdlib.h>


#include <GL/glew.h>
#include <GL/gl.h>
#include <string.h>

#include "globaldefs.h"
#include "glstates.h"

#include "matrixlib.h"
#include "camera.h"
#include "framebuffers.h"
#include "postprocess.h"
#include "depthback.h"
#include "shadermanager.h"


#include "sand.h"


framebuffer_t sandbuff;
shader_t sandvelup;
shader_t sandposup;
shader_t sandrend;

int sandsres = 0;

int sandping = 0;
		//pos, vel1, vel2
GLenum sints[] = {GL_RGB32F, GL_RGB32F, GL_RGB32F, 0, 0, 0, 0, 0};
GLenum sfmt[] = {GL_RGBA, GL_RGBA, GL_RGBA, 0, 0, 0, 0, 0};
GLenum stype[] = {GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, 0, 0, 0, 0, 0};

GLenum mbuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};

GLuint sandvao;

void sand_render(camera_t *c){
	glstate_t sp = {STATESENABLECULLFACE|STATESENABLEDEPTH, GL_ONE, GL_ONE, GL_LESS, GL_BACK, GL_TRUE, GL_LESS, 0.0, sandvao, 0, 0, 0, 0, 0, sandrend.programid, 0, {0}, {0}, {0}, {0}, {0}};
	states_forceState(sp);
	states_bindActiveTexture(2, GL_TEXTURE_2D, sandbuff.texture_id[2]);
	states_bindActiveTexture(1, GL_TEXTURE_2D, sandbuff.texture_id[1]);
	states_bindActiveTexture(0, GL_TEXTURE_2D, sandbuff.texture_id[0]);
	GLfloat mvp[16];
	Matrix4x4_ToArrayFloatGL(&c->mvp, mvp);
	glUniformMatrix4fv(sandrend.unimat40, 1, GL_FALSE, mvp);
	glDrawArrays(GL_POINTS, 0, sandsres * sandsres);
//	printf("readan %i %i\n", sandsres, sandbuff.texture_id[0]);
}

void sand_phys(void){
	//do pos update
	glBindFramebuffer(GL_FRAMEBUFFER, sandbuff.framebuffer_id);
	glViewport(0, 0, sandbuff.width, sandbuff.height);
	glDrawBuffers(1, mbuffers);
	glstate_t s = {STATESENABLECULLFACE|STATESENABLEBLEND, GL_ONE, GL_ONE, GL_LESS, GL_BACK, GL_FALSE, GL_LESS, 0.0, fsquad, 0, 0, 0, 0, 0, sandposup.programid, 0, {0}, {0}, {0}, {0}, {0}};
	states_forceState(s);
	states_bindActiveTexture(2, GL_TEXTURE_2D, sandbuff.texture_id[2]);
	states_bindActiveTexture(1, GL_TEXTURE_2D, sandbuff.texture_id[1]);
	states_bindActiveTexture(0, GL_TEXTURE_2D, sandbuff.texture_id[0]);
	glUniform1i(sandposup.uniint0, sandping);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

//vel update
	glBindFramebuffer(GL_FRAMEBUFFER, sandbuff.framebuffer_id);
	glViewport(0, 0, sandbuff.width, sandbuff.height);
	if(sandping) glDrawBuffers(1, mbuffers+1);
	else glDrawBuffers(1, mbuffers+2);
	glstate_t sp = {STATESENABLECULLFACE, GL_ONE, GL_ONE, GL_LESS, GL_BACK, GL_FALSE, GL_LESS, 0.0, fsquad, 0, 0, 0, 0, 0, sandvelup.programid, 0, {0}, {0}, {0}, {0}, {0}};
	states_forceState(sp);
	states_bindActiveTexture(2, GL_TEXTURE_2D, sandbuff.texture_id[2]);
	states_bindActiveTexture(1, GL_TEXTURE_2D, sandbuff.texture_id[1]);
	states_bindActiveTexture(0, GL_TEXTURE_2D, sandbuff.texture_id[0]);
	glUniform1i(sandvelup.uniint0, sandping);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	sandping = !sandping;

}

void sand_init(int numsands){
	sandvelup = shader_load("sandvel");
	sandposup = shader_load("sandpos");
	sandrend = shader_load("sanddraw");
	sandsres = numsands;
	sandbuff = create_framebuffer(numsands, numsands, sints, sfmt, stype, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLfloat *spos = malloc(numsands * numsands * 3 * sizeof(GLfloat));
	memset(spos, 0, numsands * numsands * 3 * sizeof(GLfloat));
	states_bindTexture(GL_TEXTURE_2D, sandbuff.texture_id[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, numsands, numsands, 0, GL_RGB, GL_FLOAT, spos);
	states_bindTexture(GL_TEXTURE_2D, sandbuff.texture_id[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, numsands, numsands, 0, GL_RGB, GL_FLOAT, spos);
	states_bindTexture(GL_TEXTURE_2D, sandbuff.texture_id[0]);
	int x, y;
	for(y = 0; y < numsands; y++){
	for(x = 0; x < numsands; x++){
		spos[(y * numsands + x) * 3+0] = (float)x / (float)numsands * 5.0 - 2.5;
		spos[(y * numsands + x) * 3+1] = (float)y / (float)numsands * 5.0 - 2.5;
		spos[(y * numsands + x) * 3+2] = 1.0;
	}}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, numsands, numsands, 0, GL_RGB, GL_FLOAT, spos);
	free(spos);

	GLfloat *sandbuff = malloc(numsands * numsands * 2 * sizeof(GLfloat));
	for(y = 0; y < numsands; y++){
	for(x = 0; x < numsands; x++){
		sandbuff[(y * numsands + x) * 2+0] = (float)x / (float)numsands;
		sandbuff[(y * numsands + x) * 2+1] = (float)y / (float)numsands;
	}}
	GLuint sandbu;
	glGenVertexArrays(1, &sandvao);
	states_bindVertexArray(sandvao);
	glGenBuffers(1, &sandbu);
	glBindBuffer(GL_ARRAY_BUFFER, sandbu);
	glBufferData(GL_ARRAY_BUFFER, numsands * numsands * 2 * sizeof(GLfloat), sandbuff, GL_STATIC_DRAW);
	free(sandbuff);
	glEnableVertexAttribArray(TCATTRIBLOC);
	glVertexAttribPointer(TCATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *) 0);
}
