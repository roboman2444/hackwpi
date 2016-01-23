#ifndef SHADERMANAGERHEADER
#define SHADERMANAGERHEADER

typedef struct shader_s {
	GLuint programid;
	GLuint texturepos[16];
	GLuint unimat40;
	GLuint unimat41;
	GLuint univec40;
	GLuint univec41;
	GLuint univec30;
	GLuint univec31;
	GLuint univec20;
	GLuint univec21;
	const char * fragfile;
	const char * vertfile;
	const char * geomfile;
} shader_t;


shader_t shader_load(const char * filename);




#endif
