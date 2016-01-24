#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <math.h>

#include "globaldefs.h"

#include "shadermanager.h"
#include "tex.h"

#include "glhelp.h"

#include "matrixlib.h"
#include "mathlib.h"
#include "camera.h"
#include "depthback.h"
#include "postprocess.h"
#include "glstates.h"
#include "fluids/fluids.h"
#include "cubemap.h"
#include "grid.h"

#include "freenect_sync/libfreenect_buffer.h"

// #define FRAMEBUFFER_ENABLE

int main(const int argc, const char ** argv){
	int width = 1280;
	int height = 720;
	if(argc > 1) width = atoi(argv[1]);
	if(argc > 2) height = atoi(argv[2]);

	init_freenect();

	GLFWwindow * window;
	if(!glfwInit()) return -1;
	glewExperimental = TRUE;
	// Set window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Generate window and set context
	window = glfwCreateWindow(width, height, "hack", NULL, NULL);
	if(!window){
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	GLenum glewError = glewInit();
	if(glewError != GLEW_OK){
		printf("Error initializing GLEW!%s\n", glewGetErrorString(glewError));
		return FALSE;
	}
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//now init other shit
	CHECKGLERROR;

	GLint maxbufattach = 0;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxbufattach);
	printf("max attachments are %i\n", maxbufattach);

	createWaveBuffer(10, 10, 2, 2);
	init_grid_shaders();

	shader_t fs = shader_load("./texturedmesh");
	GLfloat quad[] = {-1.0, -1.0, 0.0, 0.0, 0.0,
			  1.0, -1.0, 0.0, 1.0, 0.0,
			  1.0, 0.7, 0.0, 1.0, 1.0,
			  -1.0, 1.0, 0.0, 0.0, 1.0};
	GLuint quadin[] = {0, 1, 2, 0, 2, 3};


	GLuint vaoid = 0;
	GLuint bid;
	GLuint ibid;

	printf("bhaca\n");
	depth_init();
	printf("bhaca\n");
	fluids_init();
	glGenVertexArrays(1, &vaoid);
	printf("bhaca\n");
	glBindVertexArray(vaoid);
	glGenBuffers(1, &bid);
	glGenBuffers(1, &ibid);
	glBindBuffer(GL_ARRAY_BUFFER, bid);

	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(GLfloat), quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(POSATTRIBLOC);
	glVertexAttribPointer(POSATTRIBLOC, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) 0);
	glEnableVertexAttribArray(TCATTRIBLOC);
	glVertexAttribPointer(TCATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), quadin, GL_STATIC_DRAW);

		glUseProgram(fs.programid);

	struct texinfo ti;
	tex_load("test_texture.jpg", &ti);
	printf("tex id: %d\n", ti.texture);
	states_bindActiveTexture(0,GL_TEXTURE_2D, ti.texture);

	camera_t c = camera_create();

	c.pos[2] = 3.0;
	c.viewchanged = TRUE;

	float rickanglex = 0.0;
	float rickangley = 0.0;
	float rickanglez = 0.0;
	post_init(width, height);

	cube_load("cubemap/room.png");


	double lastXpos, lastYpos;
	lastXpos = lastYpos = 0;

	//render loop lol
	while (!glfwWindowShouldClose(window)) {

		#ifdef FRAMEBUFFER_ENABLE
			bind_fs();
		#endif

		states_bindActiveTexture(0, GL_TEXTURE_2D, ti.texture);
		#ifndef FRAMEBUFFER_ENABLE
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		#endif

		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

		glDepthMask(GL_FALSE);
		cube_render(&c);
		glDepthMask(GL_TRUE);

		glUseProgram(fs.programid);
		glBindVertexArray(vaoid);
		rickanglex += 0.5;
		rickangley += 0.314;
		rickanglez += 0.123;
		matrix4x4_t rickmat;
		Matrix4x4_CreateFromQuakeEntity(&rickmat, 0.0, 0.0, 0.0, rickanglex, rickangley, rickanglez, 1.0);

		matrix4x4_t outmat;
		Matrix4x4_Concat(&outmat, &c.mvp, &rickmat);
		GLfloat mvp[16];
		Matrix4x4_ToArrayFloatGL(&outmat, mvp);
		glUniformMatrix4fv(fs.unimat40, 1, GL_FALSE, mvp);
//		glClearColor(1.0, 1.0, 1.0, 1.0);
	//render shit
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		depth_update();
		depth_render(&c);

		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glUseProgram(grid_shader.programid);
		glBindVertexArray(grid_vao);
		Matrix4x4_ToArrayFloatGL(&c.mvp, mvp);
		glUniformMatrix4fv(grid_shader.unimat40, 1, GL_FALSE, mvp);
		states_bindActiveTexture(0,GL_TEXTURE_2D, ti.texture);
		states_bindActiveTexture(1, GL_TEXTURE_CUBE_MAP, cubemapid);
		glUniform3f(grid_shader.univec31, c.pos[0], c.pos[1], c.pos[2]);
		glDrawElements(GL_TRIANGLES, grid_numelements, GL_UNSIGNED_INT, 0);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		// fluids_simulate();

		#ifdef FRAMEBUFFER_ENABLE
			runpost();
		#endif
	//swap em buffs
		glfwSwapBuffers(window);
		glfwPollEvents();

		vec3_t small_right, small_forward;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT)) {
			vec3mult(small_right, c.v_right, -.1);
			vec3addvec(c.pos, c.pos, small_right);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT)) {
			vec3mult(small_right, c.v_right, .1);
			vec3addvec(c.pos, c.pos, small_right);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP)) {
			vec3mult(small_forward, c.v_forward, -.1);
			vec3addvec(c.pos, c.pos, small_forward);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN)) {
			vec3mult(small_forward, c.v_forward, .1);
			vec3addvec(c.pos, c.pos, small_forward);
		}
		c.viewchanged = TRUE;

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		c.angle[0] += (lastXpos - xpos) / 10;
		c.angle[2] += (lastYpos - ypos) / 10;

		lastXpos = xpos;
		lastYpos = ypos;

		camera_update(&c);
	}

	glfwTerminate();

	return 0;
} //todo
