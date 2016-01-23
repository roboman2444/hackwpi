#ifndef GLHELPHEADER
#define GLHEALPHEADER

#define CHECKGLERROR {if (TRUE){if (FALSE)printf("CHECKGLERROR at %s:%d\n", __FILE__, __LINE__); int checkglerror_errornumber = glGetError(); if (checkglerror_errornumber) GL_PrintError(checkglerror_errornumber, __FILE__, __LINE__);}}


void GL_PrintError(int errornumber, const char * filename, int linenumber);

#endif
