#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include "globaldefs.h"
#include "matrixlib.h"
#include "camera.h"
#include "glstates.h"
#include "shadermanager.h"
#include "cubemap.h"


#include "stb/stb_image.h"



GLuint cubemapid;

GLuint cubevao;


shader_t cubeshader;

void cube_render(camera_t *c){
	states_bindActiveTexture(0, GL_TEXTURE_CUBE_MAP, cubemapid);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapid);
	glUseProgram(cubeshader.programid);
	glBindVertexArray(cubevao);
	GLfloat mvp[16];
	Matrix4x4_ToArrayFloatGL(&c->mvronlyp, mvp);
	glUniformMatrix4fv(cubeshader.unimat40, 1, GL_FALSE, mvp);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void cube_vboload(void){
	GLfloat cubeverts[24] = {
-1.0, -1.0, 1.0,
-1.0, 1.0, 1.0,
1.0, 1.0, 1.0,
1.0, -1.0, 1.0,

-1.0, -1.0, -1.0,
-1.0, 1.0, -1.0,
1.0, 1.0, -1.0,
1.0, -1.0, -1.0
};
GLuint cubefaces[36] = { 0, 1, 2, 0, 2, 3,
			6, 5, 4, 7, 6, 4,
			0, 4, 5, 5, 1, 0,
			2, 6, 7, 3, 2, 7,
			5, 6, 2, 5, 2, 1,
			3, 7, 4, 0, 3, 4
	};


	GLuint bid;
	GLuint iid;
	glGenVertexArrays(1, &cubevao);
	glBindVertexArray(cubevao);
	glGenBuffers(1, &bid);
	glGenBuffers(1, &iid);
	glBindBuffer(GL_ARRAY_BUFFER, bid);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), cubeverts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSATTRIBLOC);
	glVertexAttribPointer(POSATTRIBLOC, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), cubefaces, GL_STATIC_DRAW);
}




void cube_load(const char * filename){
	cube_vboload();
	cubeshader = shader_load("cubemap");
	size_t filens = strlen(filename);
	char * px = malloc(filens + 4);
	char * nx = malloc(filens + 4);
	char * py = malloc(filens + 4);
	char * ny = malloc(filens + 4);
	char * pz = malloc(filens + 4);
	char * nz = malloc(filens + 4);

	sprintf(px, "%s.px", filename);
	sprintf(nx, "%s.nx", filename);
	sprintf(py, "%s.py", filename);
	sprintf(ny, "%s.ny", filename);
	sprintf(pz, "%s.pz", filename);
	sprintf(nz, "%s.nz", filename);


	glGenTextures(1, &cubemapid);
	states_bindActiveTexture(0, GL_TEXTURE_CUBE_MAP, cubemapid);
	unsigned char * img;
	GLint bpp;
	int width = 0, height = 0;
	img = stbi_load(px, &width, &height, &bpp, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	printf("%s %i %i\n", px, width, height);
	stbi_image_free(img);
	img = stbi_load(nx, &width, &height, &bpp, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	printf("%s %i %i\n", nx, width, height);
	stbi_image_free(img);
	img = stbi_load(py, &width, &height, &bpp, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	printf("%s %i %i\n", py, width, height);
	stbi_image_free(img);
	img = stbi_load(ny, &width, &height, &bpp, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	printf("%s %i %i\n", ny, width, height);
	stbi_image_free(img);
	img = stbi_load(pz, &width, &height, &bpp, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	printf("%s %i %i\n", pz, width, height);
	stbi_image_free(img);
	img = stbi_load(nz, &width, &height, &bpp, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	printf("%s %i %i\n", nz, width, height);
	stbi_image_free(img);

	int level;
	for(level = 0; level < 255; level++){
		if(1<< level > width && 1 << level > height) break;
	}
	printf("level is %i\n", level);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, level);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	free(px);
	free(nx);
	free(py);
	free(ny);
	free(pz);
	free(nz);

}
