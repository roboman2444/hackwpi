#ifndef HACKWPI_FRAMEBUFFERS_H
#define HACKWPI_FRAMEBUFFERS_H


typedef struct {
	GLuint framebuffer_id;
	GLuint texture_id;
	GLuint renderbuffer_id;
	GLuint width;
	GLuint height;
} Framebuffer;

Framebuffer create_framebuffer(int width, int height, GLenum type);
void delete_framebuffer(Framebuffer framebuffer);

#endif
