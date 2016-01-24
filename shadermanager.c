#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include "globaldefs.h"
#include "shadermanager.h"
#include "glhelp.h"


int shader_printShaderLogStatus(const GLuint id){
	int blen = 0;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &blen);
	if(blen > 1){
		GLchar *log = (GLchar *) malloc(blen);
		glGetShaderInfoLog(id, blen, 0, log);
		printf("shader log: %s\n", log);
		free(log);
		return blen;
	}
	return FALSE;
}
int shader_printProgramLogStatus(const GLuint id){
	int blen = 0;
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &blen);
	if(blen > 1){
		GLchar *log = (GLchar *) malloc(blen);
		glGetProgramInfoLog(id, blen, 0, log);
		printf("program log: %s\n", log);
		free(log);
		return blen;
	}
	return FALSE;
}

shader_t shader_load(const char *filename){
	return shader_load_fvg(filename, filename, filename);
}

shader_t shader_load_fv(const char *fragfile, const char *vertfile) {
	return shader_load_fvg(fragfile, vertfile, "\0");
}

shader_t shader_load_fvg(const char *fragfile, const char *vertfile, const char *geomfile) {
	shader_t s ={0};
	size_t fragnamesize = strlen(fragfile);
	size_t vertnamesize = strlen(vertfile);
	size_t geomnamesize = strlen(geomfile);
	s.fragfile = malloc(fragnamesize + 6);
	s.vertfile = malloc(vertnamesize + 6);
	s.geomfile = malloc(geomnamesize + 6);

	sprintf((char *)s.fragfile, "%s.frag", fragfile);
	sprintf((char *)s.vertfile, "%s.vert", vertfile);
	sprintf((char *)s.geomfile, "%s.geom", geomfile);

	FILE *ff = fopen(s.fragfile, "r");
	FILE *fv = fopen(s.vertfile, "r");
	FILE *fg = fopen(s.geomfile, "r");
	if(!ff || !fv){
		if(ff) fclose(ff);
		if(fv) fclose(fv);
		if(fg) fclose(fg);
		printf("shader(s) not avaliable: %s %s\n", s.fragfile, s.vertfile);
		return s;
	}
	fseek(ff, 0, SEEK_END);
	int ffl = ftell(ff);
	rewind(ff);
	fseek(fv, 0, SEEK_END);
	int fvl = ftell(fv);
	rewind(fv);
	if(!ff || !fv){
		if(ff) fclose(ff);
		if(fv) fclose(fv);
		if(fg) fclose(fg);
		printf("shader(s) no length: %s %s\n", s.fragfile, s.vertfile);
		return s;
	}
	int fgl = 0;
	if(fg){
		fseek(fg, 0, SEEK_END);
		fgl = ftell(fg);
		rewind(fg);
	}

	printf("shader lengths %i %i %i\n", ffl, fvl, fgl);

	char *fs = malloc(ffl +1);
	fread(fs, 1, ffl, ff);
	fs[ffl] = 0;
	char *vs = malloc(fvl +1);
	fread(vs, 1, fvl, fv);
	vs[fvl] = 0;

	char *gs = 0;
	if(fgl && fg){
		gs = malloc(fgl +1);
		fread(gs, 1, fgl, fg);
		gs[fgl] = 0;
	}

	// printf("shader contents\n%s\n", vs);
	// printf("shader contents\n%s\n", fs);
	CHECKGLERROR;
	GLuint vid = glCreateShader(GL_VERTEX_SHADER);
	CHECKGLERROR;
	GLuint fid = glCreateShader(GL_FRAGMENT_SHADER);
	CHECKGLERROR;
	glShaderSource(vid, 1, (const GLchar **)&vs, 0);
	CHECKGLERROR;
	glShaderSource(fid, 1, (const GLchar **)&fs, 0);
	CHECKGLERROR;

	GLuint gid = 0;
	if(gs){
		gid = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gid, 1, (const GLchar **) &gs, 0);
	}

	glCompileShader(vid);
	CHECKGLERROR;
	shader_printShaderLogStatus(vid);
	CHECKGLERROR;
//	shader_printProgramLogStatus(vid);
	CHECKGLERROR;

	glCompileShader(fid);
	CHECKGLERROR;
	shader_printShaderLogStatus(fid);
//	shader_printProgramLogStatus(fid);

	if(gs) glCompileShader(gid);

	if(gid)shader_printShaderLogStatus(gid);
	printf("shader ids %i %i %i\n", fid, vid, gid);


	s.programid = glCreateProgram();
	glAttachShader(s.programid, vid);
	CHECKGLERROR;
	glAttachShader(s.programid, fid);
	CHECKGLERROR;
	if(gid) glAttachShader(s.programid, gid);

	glBindFragDataLocation(s.programid, 0, "fragColor"); //todo add more later
	glBindFragDataLocation(s.programid, 1, "frag1"); //todo add more later
	glBindFragDataLocation(s.programid, 2, "frag2"); //todo add more later
	glBindFragDataLocation(s.programid, 3, "frag3"); //todo add more later
	glBindFragDataLocation(s.programid, 4, "frag4"); //todo add more later
	glBindFragDataLocation(s.programid, 5, "frag5"); //todo add more later
	glBindFragDataLocation(s.programid, 6, "frag6"); //todo add more later
	glBindFragDataLocation(s.programid, 7, "frag7"); //todo add more later
	CHECKGLERROR;

	glBindAttribLocation(s.programid, POSATTRIBLOC, "posattrib"); //todo add more later
	CHECKGLERROR;
	glBindAttribLocation(s.programid, TCATTRIBLOC, "tcattrib"); //todo add more later
	CHECKGLERROR;
	glBindAttribLocation(s.programid, OFFSETATTRIBLOC, "offsetattrib"); //todo add more later
	CHECKGLERROR;
	glLinkProgram(s.programid);
	CHECKGLERROR;
	glDeleteShader(vid);
	CHECKGLERROR;
	glDeleteShader(fid);
	if(gid)glDeleteShader(gid);
	int status = 0, fail=0;
	glGetProgramiv(s.programid, GL_LINK_STATUS, &status);
	CHECKGLERROR;
	if(status == GL_FALSE) fail = TRUE;
	if(shader_printProgramLogStatus(s.programid) || fail){
		fail = TRUE;
	}
	CHECKGLERROR;
	if(fail){
		s.programid = 0;
		return s;
	}

	glUseProgram(s.programid);
	char texstring[10];
	int i;
	for(i =0; i < 16; i++){
		sprintf(texstring, "texture%i", i);
		s.texturepos[i] = glGetUniformLocation(s.programid, texstring);
		glUniform1i(s.texturepos[i], i);
	}

	s.unimat40 = glGetUniformLocation(s.programid, "unimat40");
	s.unimat41 = glGetUniformLocation(s.programid, "unimat41");
	s.univec40 = glGetUniformLocation(s.programid, "univec40");
	s.univec41 = glGetUniformLocation(s.programid, "univec41");
	s.univec30 = glGetUniformLocation(s.programid, "univec30");
	s.univec31 = glGetUniformLocation(s.programid, "univec31");
	s.univec20 = glGetUniformLocation(s.programid, "univec20");
	s.univec21 = glGetUniformLocation(s.programid, "univec21");
	s.uniint0 = glGetUniformLocation(s.programid, "uniint0");

	if(gs) free(gs);
	if(vs) free(vs);
	if(fs) free(fs);

	if(ff) fclose(ff);
	if(fv) fclose(fv);
	if(fg) fclose(fg);


	return s;

}
