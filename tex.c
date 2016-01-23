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
    GLuint texture;
    int width, height;
    unsigned char *image;
    GLint bpp;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    image = stbi_load(fn, &width, &height, &bpp, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, (bpp == 3)?GL_RGB:GL_RGBA, width, height,
                 0, (bpp == 3)?GL_RGB:GL_RGBA, GL_UNSIGNED_BYTE, image);
    //glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    ti->texture = texture;
    ti->width = width;
    ti->height = height;
    ti->format = bpp;
}
