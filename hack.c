#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "globaldefs.h"

#include "shadermanager.h"

#include "glhelp.h"

int main(const int argc, const char ** argv){
	int width = 1280;
	int height = 720;
	if(argc > 1) width = atoi(argv[1]);
	if(argc > 2) height = atoi(argv[2]);


	GLFWwindow * window;
	if(!glfwInit()) return -1;

	// Set window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Generate window and set context
	window = glfwCreateWindow(width, height, "hack", NULL, NULL);
	if(!window){
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	GLenum glewError = glewInit();
	if(glewError != GLEW_OK){
		printf("Error initializing GLEW!%s\n", glewGetErrorString(glewError));
		return FALSE;
	}
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
	//now init other shit
	CHECKGLERROR;

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
	glVertexAttribPointer(TCATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (2 * sizeof(GLfloat)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), quadin, GL_STATIC_DRAW);

		glUseProgram(fs.programid);
	//render loop lol
	while (!glfwWindowShouldClose(window)) {
//		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
	//render shit
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 3, 0);
	//swap em buffs
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}

	glfwTerminate();

	return 0;
} //todo
