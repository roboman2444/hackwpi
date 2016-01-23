#include "GL/glew.h"
#include "GL/gl.h"
#include "stb/stb_image.h"

#include "globaldefs.h"
#include "tex.h"


/* tex_load: loads and binds a texture from a filename into the current OpenGL
 *  context.
 *
 * If ti is NULL, this routine *will* segfault.
 */
void tex_load(const char * fn, struct texinfo *ti)
{
    GLuint texture = 0;
    int width = 0, height = 0;
    unsigned char *image;
    GLint bpp;


    image = stbi_load(fn, &width, &height, &bpp, 0);
	if(!image) return;
    printf("tex %s w: %i, h: %i, bpp: %i\n", fn, width, height, bpp);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);


int level;
	for(level = 0; level < 255; level++){
		if(1 << level > width && 1<<level > height) break;
	}

    glTexImage2D(GL_TEXTURE_2D, 0, (bpp == 3)?GL_RGB:GL_RGBA, width, height,
                 0, (bpp == 3)?GL_RGB:GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, level);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    ti->texture = texture;
    ti->width = width;
    ti->height = height;
    ti->format = bpp;
}
