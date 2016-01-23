#ifndef DEPTHBACKHEADER
#define DEPTHBACKHEADER

GLuint depthwidth;
GLuint depthheight;

GLuint depthtexid;

int depthneedsupdate;


void depth_render(camera_t *c);
void depth_update(void);
void depth_init(void);



#endif
