#include <GL/glew.h>
#include <GL/gl.h>

#include "framebuffers.h"
#include "globaldefs.h"

#include "glstates.h"

framebuffer_t create_framebuffer(int width, int height, GLenum texturesint[8],  GLenum texturesfmt[8], GLenum texturestype[8], int renderbuff) {
	framebuffer_t buffer;

	glGenFramebuffers(1, &buffer.framebuffer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer.framebuffer_id);

	int i;
	for(i = 0; i <8; i++){
		if(!texturesint[i]) continue;
		glGenTextures(1, &buffer.texture_id[i]);
		states_bindActiveTexture(0, GL_TEXTURE_2D, buffer.texture_id[i]);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			texturesint[i],
			width,
			height,
			0,
			texturesfmt[i],
			texturestype[i],
			NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0 + i,
			GL_TEXTURE_2D,
			buffer.texture_id[i],
			0
		);
	}
	if(renderbuff){
		glGenRenderbuffers(1, &buffer.renderbuffer_id);
		glBindRenderbuffer(GL_RENDERBUFFER, buffer.renderbuffer_id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(
			GL_FRAMEBUFFER,
			GL_DEPTH_STENCIL_ATTACHMENT,
			GL_RENDERBUFFER,
			buffer.renderbuffer_id
		);
	}
	buffer.width = width;
	buffer.height = height;
	return buffer;
}

void delete_framebuffer(framebuffer_t framebuffer) {
	glDeleteRenderbuffers(1, &framebuffer.renderbuffer_id);
	int i;
	for(i = 0; i < 8; i++) if(framebuffer.texture_id[i])glDeleteTextures(1, &framebuffer.texture_id[i]);
	glDeleteFramebuffers(1, &framebuffer.framebuffer_id);
}
