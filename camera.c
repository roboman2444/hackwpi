#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#include "globaldefs.h"


#include "matrixlib.h"
#include "camera.h"

camera_t camera_create(void){
        camera_t cam = {0};
        cam.fov = 90.0f;
        cam.near = 0.1f;
        cam.far = 100.0f;
        cam.aspect = 16.0f/9.0f;
        cam.viewchanged = TRUE;
        cam.projchanged = TRUE;
        Matrix4x4_CreateIdentity(&cam.obj);
        Matrix4x4_CreateIdentity(&cam.view);
        Matrix4x4_CreateIdentity(&cam.proj);
        Matrix4x4_CreateIdentity(&cam.mvp);
	return cam;
}


void updateproj(camera_t *c){
	double sine, cotangent, deltaZ;
	double radians = c->fov / 2.0 * M_PI / 180.0;
	deltaZ = c->far - c->near;
	sine = sin(radians);
	cotangent = cos(radians)/sine;
	memset(&c->proj, 0, sizeof(matrix4x4_t));
	c->proj.m[0][0] = cotangent / c->aspect;
	c->proj.m[1][1] = cotangent;
	c->proj.m[2][2] = -(c->far + c->near) / deltaZ;
	c->proj.m[2][3] = -1.0;
	c->proj.m[3][2] = -2.0 * c->near * c->far / deltaZ;

}
void updateview(camera_t *c){
	Matrix4x4_CreateFromQuakeEntity(&c->obj, c->pos[0], c->pos[1], c->pos[2], c->angle[0], c->angle[1], c->angle[2], 1.0);
	Matrix4x4_Invert_Simple(&c->view, &c->obj);
	Matrix4x4_CopyRotateOnly(&c->ronly, &c->view);
	//update v->forward;
	vec3_t inf = {0.0, 0.0, 1.0};
	Matrix4x4_Transform(&c->obj, inf, c->v_forward);
	vec3_t inu = {0.0, 1.0,0.0};
	Matrix4x4_Transform(&c->obj, inu, c->v_up);
	vec3_t inr = {1.0, 0.0, 0.0};
	Matrix4x4_Transform(&c->obj, inr, c->v_right);
}
void camera_update(camera_t *c){
	if(c->viewchanged) updateview(c);
	if(c->projchanged) updateproj(c);
	if(c->viewchanged || c->projchanged){
		Matrix4x4_Concat(&c->mvp, &c->proj, &c->view);
		Matrix4x4_Concat(&c->mvronlyp, &c->proj, &c->ronly);
	}
	c->viewchanged = FALSE;
	c->projchanged = TRUE;
}
