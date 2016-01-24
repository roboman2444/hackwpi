#ifndef CAMERAHEADER
#define CAMERAHEADER


typedef struct camera_s {
	matrix4x4_t proj;
	matrix4x4_t obj;
	matrix4x4_t view;
	matrix4x4_t mvp;
	matrix4x4_t ronly;
	matrix4x4_t mvronlyp;
	vec3_t v_forward;
	vec3_t v_up;
	vec3_t v_right;
	vec3_t pos;
	vec3_t angle;
	float aspect;
	float fov;
	float near;
	float far;
	int viewchanged;
	int projchanged;
} camera_t;



camera_t camera_create(void);
void camera_update(camera_t *c);

#endif

