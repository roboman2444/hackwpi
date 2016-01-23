#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

#include <GL/gl.h>

#include "globaldefs.h"



void GL_PrintError(int errornumber, const char *filename, int linenumber){
        switch(errornumber){
#ifdef GL_INVALID_ENUM
        case GL_INVALID_ENUM:
                printf("GL_INVALID_ENUM at %s:%i\n", filename, linenumber);
                break;
#endif
#ifdef GL_INVALID_VALUE
        case GL_INVALID_VALUE:
                printf("GL_INVALID_VALUE at %s:%i\n", filename, linenumber);
                break;
#endif
#ifdef GL_INVALID_OPERATION
        case GL_INVALID_OPERATION:
                printf("GL_INVALID_OPERATION at %s:%i\n", filename, linenumber);
                break;
#endif
#ifdef GL_STACK_OVERFLOW
        case GL_STACK_OVERFLOW:
                printf("GL_STACK_OVERFLOW at %s:%i\n", filename, linenumber);
                break;
#endif
#ifdef GL_STACK_UNDERFLOW
        case GL_STACK_UNDERFLOW:
                printf("GL_STACK_UNDERFLOW at %s:%i\n", filename, linenumber);
                break;
#endif
#ifdef GL_OUT_OF_MEMORY
        case GL_OUT_OF_MEMORY:
                printf("GL_OUT_OF_MEMORY at %s:%i\n", filename, linenumber);
                break;
#endif
#ifdef GL_TABLE_TOO_LARGE
        case GL_TABLE_TOO_LARGE:
                printf("GL_TABLE_TOO_LARGE at %s:%i\n", filename, linenumber);
                break;
#endif
#ifdef GL_INVALID_FRAMEBUFFER_OPERATION
        case GL_INVALID_FRAMEBUFFER_OPERATION:
                printf("GL_INVALID_FRAMEBUFFER_OPERATION at %s:%i\n", filename, linenumber);
                break;
#endif
        default:
                printf("GL UNKNOWN (%i) at %s:%i\n", errornumber, filename, linenumber);
                break;
        }
}

