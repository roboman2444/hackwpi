#ifndef HACKWPI_TEX_H
#define HACKWPI_TEX_H

#include <GL/gl.h>

struct texinfo {
    GLuint texture;
    int width;
    int height;
    GLint format;
};

void tex_load(const char * fn, struct texinfo *ti);

#endif
