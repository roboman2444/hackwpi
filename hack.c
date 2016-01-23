#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "globaldefs.h"

#include "shadermanager.h"

int main(const int argc, const char ** argv){
	int width = 1280;
	int height = 720;
	if(argc > 1) width = atoi(argv[1]);
	if(argc > 2) height = atoi(argv[2]);


	GLFWwindow * window;
	if(!glfwInit()) return -1;

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

	GLenum glewError = glewInit();
	if(glewError != GLEW_OK){
		printf("Error initializing GLEW!%s\n", glewGetErrorString(glewError));
		return FALSE;
	}
	//now init other shit

	shader_t fs = shader_load("./texturedmesh");
	GLfloat quad[] = {-1.0, -1.0, 0.0, 0.0, 0.0,
			  1.0, -1.0, 0.0, 1.0, 0.0,
			  1.0, 0.7, 0.0, 1.0, 1.0,
			  -1.0, 1.0, 0.0, 0.0, 1.0};
	GLuint quadin[] = {0, 1, 2, 0, 2, 3};
	glEnableVertexAttribArray(POSATTRIBLOC);
	glVertexAttribPointer(POSATTRIBLOC, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), quad);
	glEnableVertexAttribArray(TCATTRIBLOC);
	glVertexAttribPointer(TCATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), quad + 2 * sizeof(GL_FLOAT));

	//render loop lol
	while (!glfwWindowShouldClose(window)) {
		glUseProgram(fs.programid);
	//render shit
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, quadin);
	//swap em buffs
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}

	glfwTerminate();

	return 0;
} //todo
