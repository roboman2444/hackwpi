#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "globaldefs.h"

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
	//gl is all set up
	return 0;
} //todo
