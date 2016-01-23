#include "stb/stb_image.h"

#include "global_defs.h"
#include "tex.h"


/* tex_load: loads and binds a texture from a filename into the current OpenGL
 *  context.
 *
 * If ti is NULL, this routine *will* segfault.
 */
void tex_load(const char * fn, struct texinfo *ti)
{
    GLuint texture;
    int width, height;
    unsigned char *image;
    unsigned int bpp;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    image = stb_load(fn, &width, &height, &bpp, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, (bpp == 3)?GL_RGB:GL_RGBA, width, height,
                 0, (bpp == 3)?GL_RGB:GL_RGBA, GL_UNSIGNED_BYTE, image);
    //glGenerateMipmap(GL_TEXTURE_2D);
    stb_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    ti.texture = texture;
    ti.width = width;
    ti.height = height;
    ti.format = bpp;
}
