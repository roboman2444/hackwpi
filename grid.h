#include <GL/gl.h>
#include "shadermanager.h"

GLuint grid_vao, grid_vertices, grid_elements, grid_numelements;
shader_t grid_shader;

void init_grid_shaders();
void createWaveBuffer(const int x, const int y, const float scalex, const float scaley);