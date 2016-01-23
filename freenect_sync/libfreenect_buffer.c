#include "libfreenect.h"
#include "libfreenect_sync.h"
#include "libfreenect_buffer.h"

char *freenect_sync_get_depth_buffer(int index) {
	static char *data = 0;
	unsigned int timestamp;
	if (freenect_sync_get_depth((void**)&data, &timestamp, index, FREENECT_DEPTH_11BIT))
	    return 0;
	return data;
}

char *freenect_sync_get_color_buffer(int index) {
	static char *data = 0;
	unsigned int timestamp;
	if (freenect_sync_get_video((void**)&data, &timestamp, index, FREENECT_VIDEO_RGB))
	    return 0;
	return data;
}