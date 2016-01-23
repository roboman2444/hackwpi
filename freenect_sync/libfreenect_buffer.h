#ifndef LIBFREENECT_BUFFERHEADER
#define LIBFREENECT_BUFFERHEADER

#define KINECT_INDEX 0

/**
 *  Returns a pointer to a buffer.
 *  The depth is stored in an 11 bit unsigned int byte-aligned to 2 bytes
 *  The color is stored in as a 24 bit RGB value
 *  index refers to the kinect camera index
 */
char *freenect_sync_get_depth_buffer(int index);
int freenect_sync_update_depth_buffer(char *depth_buffer, int index);

char *freenect_sync_get_color_buffer(int index);

#endif // LIBFREENECT_BUFFERHEADER
