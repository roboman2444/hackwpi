#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glfw3.h>
int main(void){
	GLFWwindow * window;
	if(!glfwInit()) return -1;
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
