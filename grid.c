#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include "globaldefs.h"
#include "grid.h"
#include "shadermanager.h"

void init_grid_shaders() {
	grid_shader = shader_load("grid");
}

void createWaveBuffer(const int x, const int y, const float scalex, const float scaley){
    unsigned int numverts = x * y;
    GLfloat * wavebuffer = malloc(numverts * 5 * sizeof(GLfloat));
    GLfloat * wavebuffert = malloc(numverts * 5 * sizeof(GLfloat));
    int ix;
    int iy;
    for(iy = 0; iy < y; iy++){
    for(ix = 0; ix < x; ix++){
        wavebuffer[((iy * x) + ix) * 5] = (((float) ix/(float)x) - 0.5) * scalex * 2.0;
        wavebuffer[(((iy * x) + ix) *5)+  1] = (((float) iy/(float)y) - 0.5) * scaley * 2.0;
        wavebuffer[(((iy * x) + ix) * 5) + 2] = 0.0f;
        wavebuffer[(((iy * x) + ix) * 5) + 3] = ix/ (float) x;
        wavebuffer[(((iy * x) + ix) * 5) + 4] = iy / (float) y;
//        wavebuffer[(((iy * x) + ix) * 3) + 2] = sin(iy /30.0);
    }}
    memcpy(wavebuffert, wavebuffer, numverts * 5 * sizeof(GLfloat));    int fx = x-1;
    int fy = y-1;
    unsigned int numfaces = fy * fx * 2;
    GLuint * facebuffer = malloc(numfaces * 3 * sizeof(GLuint));
    for(iy = 0; iy < fy; iy++){
    	for(ix = 0; ix < fx; ix++){
	        unsigned int topleft = (iy * x) + ix;
	        unsigned int topright = topleft + 1;
	        unsigned int bottomright = topright + x;
	        unsigned int bottomleft = topleft + x;
	        GLuint * fb = &facebuffer[((iy * fx) + ix) * 6];
	        fb[0] = bottomleft;
	        fb[1] = topleft;
	        fb[2] = topright;
	        fb[3] = bottomleft;
	        fb[4] = topright;
	        fb[5] = bottomright;
	    }
	}
    glGenVertexArrays(1, &grid_vao);
    states_bindVertexArray(grid_vao);
    glGenBuffers(1, &grid_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, grid_vertices);
    glBufferData(GL_ARRAY_BUFFER, numverts * 5 * sizeof(GLfloat), wavebuffer, GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSATTRIBLOC);
    glVertexAttribPointer(POSATTRIBLOC, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(TCATTRIBLOC);
    glVertexAttribPointer(TCATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    glGenBuffers(1, &grid_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numfaces * 3 * sizeof(GLuint), facebuffer, GL_STATIC_DRAW);
    grid_numelements = numfaces * 3;
}
