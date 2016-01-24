
#ifndef LIBFREENECT_BUFFERHEADER
#define LIBFREENECT_BUFFERHEADER

#define KINECT_INDEX 0

#include <pthread.h>
#include <libfreenect.h>

volatile uint16_t *rawdepthdata;
volatile int depth_data_ready;
volatile int fn_alive;

pthread_mutex_t depth_mutex;
pthread_t freenect_thread;

freenect_context *fn_context;
freenect_device *fn_device;
int fn_devices;

int init_freenect();

void *freenect_threadfunc(void *arg);
/**
 *  Returns a pointer to a buffer.
 *  The depth is stored in an 11 bit unsigned int byte-aligned to 2 bytes
 *  The color is stored in as a 24 bit RGB value
 *  index refers to the kinect camera index
 */
void update_depth_buffer(freenect_device *dev, void *v_depth, uint32_t timestamp);


#endif // LIBFREENECT_BUFFERHEADER
