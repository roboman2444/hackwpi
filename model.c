#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <string.h>

#include "globaldefs.h"
#include "iqm.h"


#include "model.h"


model_t loadiqmmeshes(const struct iqmheader hdr, unsigned char * buf){
	printf("loadiqm meshes n shiiiiiet\n");
	model_t m ={0};
	m.numfaces = hdr.num_triangles;
	m.numverts = hdr.num_vertexes;
	float *pos = 0;
	float * norm = 0;
	float * texcoord = 0;
	float * tangent = 0;
	struct iqmvertexarray * var = (struct iqmvertexarray *) &buf[hdr.ofs_vertexarrays];
	int i;
	for(i = 0; i < (int) hdr.num_vertexarrays; i++){
		struct iqmvertexarray va = var[i];
		switch(va.type){
			case IQM_POSITION: pos = (float *) &buf[va.offset]; break;
			case IQM_NORMAL: norm = (float *) &buf[va.offset]; break;
			case IQM_TEXCOORD: texcoord = (float *) &buf[va.offset]; break;
			case IQM_TANGENT: tangent = (float *) &buf[va.offset]; break;
			default: break;
		}
	}
	if(!pos) return m;
	glGenVertexArrays(1, &m.vao);
	states_bindVertexArray(m.vao);
	GLuint posb = 0;
	GLuint normb = 0;
	GLuint texcoordb = 0;
	GLuint tangentb = 0;
	if(pos){
		glGenBuffers(1, &posb);
		glBindBuffer(GL_ARRAY_BUFFER, posb);
		glBufferData(GL_ARRAY_BUFFER, m.numverts * 3 * sizeof(GLfloat), pos, GL_STATIC_DRAW);
		glEnableVertexAttribArray(POSATTRIBLOC);
		glVertexAttribPointer(POSATTRIBLOC, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0);
	}
	if(norm){
		glGenBuffers(1, &normb);
		glBindBuffer(GL_ARRAY_BUFFER, normb);
		glBufferData(GL_ARRAY_BUFFER, m.numverts * 3 * sizeof(GLfloat), norm, GL_STATIC_DRAW);
		glEnableVertexAttribArray(NORMATTRIBLOC);
		glVertexAttribPointer(NORMATTRIBLOC, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0);
	}
	if(texcoord){
		glGenBuffers(1, &texcoordb);
		glBindBuffer(GL_ARRAY_BUFFER, texcoordb);
		glBufferData(GL_ARRAY_BUFFER, m.numverts * 2 * sizeof(GLfloat), texcoord, GL_STATIC_DRAW);
		glEnableVertexAttribArray(TCATTRIBLOC);
		glVertexAttribPointer(TCATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *) 0);
	}
	if(tangent){
		glGenBuffers(1, &tangentb);
		glBindBuffer(GL_ARRAY_BUFFER, tangentb);
		glBufferData(GL_ARRAY_BUFFER, m.numverts * 3 * sizeof(GLfloat), tangent, GL_STATIC_DRAW);
		glEnableVertexAttribArray(TANGENTATTRIBLOC);
		glVertexAttribPointer(TANGENTATTRIBLOC, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0);
	}
	GLuint *tris = (GLuint *) &buf[hdr.ofs_triangles];
	//MAY NEED TO FLIP FACES HERE
	GLuint faceid;
	glGenBuffers(1, &faceid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.numfaces * 3 * sizeof(GLuint), tris, GL_STATIC_DRAW);
	printf("loaded %i verts and %i faces ", m.numverts, m.numfaces);
	return m;


}

model_t model_loadIQM(const char * filename){
	model_t m = {0};
	FILE *f = fopen(filename, "rb");
	if(!f) return m;
	unsigned char * buf = NULL;
	struct iqmheader hdr;
	if(fread(&hdr, 1, sizeof(hdr), f) != sizeof(hdr) || memcmp(hdr.magic, IQM_MAGIC, sizeof(hdr.magic))) goto error;
	if(hdr.version != IQM_VERSION) goto error;
	if(hdr.filesize > (16<<20)) goto error;
	buf = malloc(hdr.filesize);
	if(!buf) goto error;
	memcpy(buf, &hdr, sizeof(hdr));
	if(fread(buf+sizeof(hdr), 1, hdr.filesize - sizeof(hdr), f) != hdr.filesize - sizeof(hdr)) goto error;
	if(hdr.num_meshes <=0) goto error;
	m = loadiqmmeshes(hdr, buf);
	if(!m.numverts) goto error;
	printf(" from model %s\n", filename);
	done:
	if(buf) free(buf);
	if(f)fclose(f);
	return m;
	error:
	printf("error loading model %s\n", filename);
	memset(&m, 0, sizeof(m));
	goto done;
}
