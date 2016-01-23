#ifndef HACKWPI_FRAMEBUFFERS_H
#define HACKWPI_FRAMEBUFFERS_H


typedef struct framebuffer_s {
	GLuint framebuffer_id;
	GLuint texture_id[8];
	GLenum texture_comp[8];
	GLenum texture_type[8];
	GLuint renderbuffer_id;
	GLuint width;
	GLuint height;
} framebuffer_t;

framebuffer_t create_framebuffer(int width, int height, GLenum texturesint[8], GLenum texturesfmt[8], GLenum texturestype[8], int render);
void delete_framebuffer(framebuffer_t fb);

#endif
