#ifndef POSTPROCESSHEADER
#define POSTPROCESSHEADER

void post_init(int width, int height);

void bind_fs(void);

void runpost(void);

GLuint fsquad;

#endif
