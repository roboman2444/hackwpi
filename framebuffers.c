#include <GL/glew.h>
#include <GL/gl.h>

#include "framebuffers.h"
#include "globaldefs.h"

Framebuffer create_framebuffer(int width, int height, GLenum type) {
	Framebuffer buffer;

	glGenFramebuffers(1, &buffer.framebuffer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer.framebuffer_id);

	glGenTextures(1, &buffer.texture_id);
	glBindTexture(GL_TEXTURE_2D, buffer.texture_id);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		width,
		height,
		0,
		GL_RGB,
		type,
		NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		buffer.texture_id,
		0
	);

	glGenRenderbuffers(1, &buffer.renderbuffer_id);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer.renderbuffer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER,
		buffer.renderbuffer_id
	);

	return buffer;
}

void delete_framebuffer(Framebuffer framebuffer) {
	glDeleteRenderbuffers(1, &framebuffer.renderbuffer_id);
	glDeleteTextures(1, &framebuffer.texture_id);
	glDeleteFramebuffers(1, &framebuffer.framebuffer_id);
}
