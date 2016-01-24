#include <stdio.h>
#include <unistd.h>

#include "../globaldefs.h"

#include "libfreenect_buffer.h"

int init_freenect() {
	pthread_mutex_init(&depth_mutex, NULL);

	// Initialize libfreenect.
	if (freenect_init(&fn_context, NULL) < 0) {
		printf("Error: Freenect context failed.\n");
		return -1;
	}

	fn_devices = freenect_num_devices (fn_context);
	printf ("%d freenect devices found\n", fn_devices);
	if (fn_devices == 0) {
		printf("Error: No freenect devices found\n");
		freenect_shutdown(fn_context);
		return -1;
	}
	
	if (freenect_open_device(fn_context, &fn_device, KINECT_INDEX) < 0) {
		printf("Error: Could not open freenect device\n");
		freenect_shutdown(fn_context);
		return -1;
	}

	if (pthread_create(&freenect_thread, NULL, freenect_threadfunc, NULL)) {
		printf("Error: pthread_create failed\n");
		freenect_shutdown(fn_context);
		return -1;
	}
	
	printf("3\n");
	fflush(stderr);
	fn_alive = TRUE;
	pthread_create(&freenect_thread, NULL, freenect_threadfunc, NULL);

	printf("1\n");
	fflush(stderr);
	return 0;
}


void *freenect_threadfunc(void *arg) {
	// int accelCount = 0;
	freenect_set_led(fn_device, LED_BLINK_GREEN);
	freenect_set_depth_callback(fn_device, update_depth_buffer);
	freenect_set_depth_mode(fn_device, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT));
	freenect_start_depth(fn_device);
	
	while (fn_alive && freenect_process_events(fn_context) >= 0) {
		/*//Throttle the text output
		if (accelCount++ >= 2000) {
			accelCount = 0;
			freenect_raw_tilt_state* state;
			freenect_update_tilt_state(f_dev);
			state = freenect_get_tilt_state(f_dev);
			double dx,dy,dz;
			freenect_get_mks_accel(state, &dx, &dy, &dz);
			printf("\r raw acceleration: %4d %4d %4d  mks acceleration: %4f %4f %4f", state->accelerometer_x, state->accelerometer_y, state->accelerometer_z, dx, dy, dz);
			fflush(stdout);
		}*/
		usleep(16000);
	}

	printf("\nshutting down streams...\n");

	freenect_stop_depth(fn_device);

	freenect_close_device(fn_device);
	freenect_shutdown(fn_context);

	return NULL;
}

void update_depth_buffer(freenect_device *dev, void *v_depth, uint32_t timestamp) {
	pthread_mutex_lock(&depth_mutex);
	rawdepthdata = (uint16_t*)v_depth;
	depth_data_ready = TRUE;
	pthread_mutex_unlock(&depth_mutex);
}
