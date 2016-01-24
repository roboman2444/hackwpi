#include <stdio.h>
#include <stdlib.h>


#include <GL/glew.h>
#include <GL/gl.h>

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

int sandsres = 0;

int sandping = 0;
		//pos, vel1, vel2
GLenum sints[] = {GL_RGB16F, GL_RGB16F, GL_RGB16F, 0, 0, 0, 0, 0};
GLenum sfmt[] = {GL_RGB, GL_RGB, GL_RGB, 0, 0, 0, 0, 0};
GLenum stype[] = {GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, 0, 0, 0, 0, 0};

GLenum mbuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};

void sand_phys(void){
	//do pos update
	glBindFramebuffer(GL_FRAMEBUFFER, sandbuff.framebuffer_id);
	glViewport(0, 0, sandbuff.width, sandbuff.height);
	glDrawBuffers(1, mbuffers);
	glstate_t s = {STATESENABLECULLFACE|STATESENABLEBLEND, GL_ONE, GL_ONE, GL_LESS, GL_BACK, GL_FALSE, GL_LESS, 0.0, fsquad, 0, 0, 0, 0, 0, sandposup.programid, 0, {0}, {0}, {0}, {0}, {0}};
	states_forceState(s);
	states_bindActiveTexture(0, GL_TEXTURE_2D, sandbuff.texture_id[0]);
	states_bindActiveTexture(1, GL_TEXTURE_2D, sandbuff.texture_id[1]);
	states_bindActiveTexture(2, GL_TEXTURE_2D, sandbuff.texture_id[2]);
	glUniform1i(sandposup.uniint0, sandping);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

//vel update
	glBindFramebuffer(GL_FRAMEBUFFER, sandbuff.framebuffer_id);
	glViewport(0, 0, sandbuff.width, sandbuff.height);
	if(sandping) glDrawBuffers(1, mbuffers+1);
	else glDrawBuffers(1, mbuffers+2);
	glstate_t sp = {STATESENABLECULLFACE|STATESENABLEBLEND, GL_ONE, GL_ONE, GL_LESS, GL_BACK, GL_FALSE, GL_LESS, 0.0, fsquad, 0, 0, 0, 0, 0, sandvelup.programid, 0, {0}, {0}, {0}, {0}, {0}};
	states_forceState(sp);
	states_bindActiveTexture(0, GL_TEXTURE_2D, sandbuff.texture_id[0]);
	states_bindActiveTexture(1, GL_TEXTURE_2D, sandbuff.texture_id[1]);
	states_bindActiveTexture(2, GL_TEXTURE_2D, sandbuff.texture_id[2]);
	glUniform1i(sandvelup.uniint0, sandping);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	sandping = !sandping;
}

void sand_init(int numsands){
	sandvelup = shader_load("sandvel");
	sandposup = shader_load("sandpos");
	sandsres = numsands;
	sandbuff = create_framebuffer(sandsres, sandsres, sints, sfmt, stype, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, sandbuff.framebuffer_id);
	glViewport(0, 0, sandbuff.width, sandbuff.height);
	glClear(GL_COLOR_BUFFER_BIT);
}
