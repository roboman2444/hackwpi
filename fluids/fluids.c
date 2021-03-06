#include <stdio.h>
#include <stdlib.h>


#include <GL/glew.h>
#include <GL/gl.h>

#include "../globaldefs.h"
#include "../shadermanager.h"
#include "../matrixlib.h"
#include "../camera.h"
#include "fluids.h"

#include "../glstates.h"



#define BUFFER_OFFSET(i) ((char*)NULL + (i))

shader_t mainShader, addForce, advectVelocity, divergenceShader, jacobiSolver,
		 jacobiSolverBound, quadProgram, subtractPressure,
		 texCopyShader, velocityBoundary;

GLuint All_screen, All_screenVBO;
GLuint boundary, boundaryVBO, boundaryIndexVBO;
GLuint inside, insideVBO, insideIndexVBO;

GLuint quad_vertexbuffer;

GLuint Velocity0, Velocity1, Pressure0, 
	   Pressure1, Jacobi_iter_FBO_1, 
	   Jacobi_iter_FBO_2, divergence, MainFBO;

GLuint fluidswidth = 640;
GLuint fluidsheight = 480;
GLenum drawBuffers[5];

int fluids_init(void){
	// Shaders loaded
	mainShader 			= shader_load("fluids/MainShader");
	addForce 			= shader_load_fv("fluids/addForce", "fluids/MainShader");
	advectVelocity 		= shader_load_fv("fluids/advectVelocity", "fluids/MainShader");
	velocityBoundary 	= shader_load_fv("fluids/velocityBoundary", "fluids/boundary");
	divergenceShader 	= shader_load_fv("fluids/divergence", "fluids/MainShader");
	jacobiSolver 		= shader_load_fv("fluids/jacobiSolver", "fluids/MainShader");
	jacobiSolverBound 	= shader_load_fv("fluids/jacobiSolver", "fluids/boundary");
	quadProgram 		= shader_load("fluids/quadProgram");
	subtractPressure 	= shader_load_fv("fluids/subtractPressureGradient", "fluids/MainShader");
	texCopyShader 		= shader_load_fv("fluids/texCopyShader", "fluids/MainShader");
	//Create Vertex Array Object
	glGenVertexArrays(1, &All_screen);
	states_bindVertexArray(All_screen);

	GLfloat canvas[] = {		//DATA
		-1.0f,-1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f,
		-1.0f, 1.0f
	};	//Don't need index data for this peasant mesh!

	//VBO for fluid wall
	glGenBuffers(1, &All_screenVBO);
	states_bindBuffer(GL_ARRAY_BUFFER, All_screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(canvas), &canvas, GL_STATIC_DRAW);
	//Assign attribs
	glVertexAttribPointer(POSATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(POSATTRIBLOC);
	states_bindVertexArray(0);	//unbind VAO

	// Create vaos for the inside and boundary cells
	glGenVertexArrays(1, &inside);
	states_bindVertexArray(inside);

	float px = 1.0 / fluidswidth; px = 15 * px;
	float py = 1.0 / fluidsheight;  py = 15 * py;
	float x = 1 - px;
	float y = 1 - py;
	// Another VBO (this one is for fluid)
	GLfloat fluidwall[] = {		//DATA
	-x,-y,
	-x, y,
	 x,-y,
	 x,-y,
	 x, y,
	-x, y
	};	//Don't need index data for this peasant mesh!
	GLuint fluid;//VBO for fluid wall
	glGenBuffers(1, &fluid);
	states_bindBuffer(GL_ARRAY_BUFFER, fluid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fluidwall), &fluidwall, GL_STATIC_DRAW);
	//Assign attribs
	glVertexAttribPointer(POSATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(POSATTRIBLOC);
	states_bindVertexArray(0);	//unbind VAO


	//Another VAO for boundary
	glGenVertexArrays(1, &boundary);
	states_bindVertexArray(boundary);
	GLfloat boundaryWall[] = {

		//left
		-1, 1, 1, 0,
		px - 1, 1, 1, 0,
		-1, -1, 1, 0,
		px - 1, 1, 1, 0,
		px - 1, -1, 1, 0,
		-1, -1, 1, 0,
		//bottom
		-1,py-1, 0, 1,
		-1, -1, 0, 1,
		1, -1, 0, 1,
		-1, py - 1, 0, 1,
		1, py - 1, 0, 1,
		1, -1, 0, 1,
		//right
		1-px,1, -1,0,
		1, 1, -1, 0,
		1 - px, -1, -1, 0,
		1, 1, -1, 0,
		1 - px, -1, -1, 0,
		1, -1, -1, 0,
		//up
		-1,1,0,-1,
		1, 1, 0, -1,
		-1, 1 - py, 0, -1,
		1, 1, 0, -1,
		1, 1 - py, 0, -1,
		-1, 1 - py, 0, -1,
	};

	//VBO for fluid wall
	glGenBuffers(1, &boundaryVBO);
	states_bindBuffer(GL_ARRAY_BUFFER, boundaryVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boundaryWall), &boundaryWall, GL_STATIC_DRAW);

	//Assign attribs
	glVertexAttribPointer(POSATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GL_FLOAT), 0);
	glEnableVertexAttribArray(POSATTRIBLOC);
	glVertexAttribPointer(OFFSETATTRIBLOC, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GL_FLOAT), BUFFER_OFFSET(2*sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(OFFSETATTRIBLOC);
	states_bindVertexArray(0);	//unbind VAO

	glGenFramebuffers(1,&MainFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, MainFBO);

	//now create a texture
	GLuint renderTexture;
	glGenTextures(1, &renderTexture);
	states_bindTexture(GL_TEXTURE_2D, renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, fluidswidth, fluidsheight, 0, GL_RGBA, GL_FLOAT, 0);
	//filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//set renderTexture as our color attachment#0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, renderTexture, 0);

	// The depth renderbuffer
	GLuint depthbuffer;
	glGenTextures(1, &depthbuffer);
	states_bindTexture(GL_TEXTURE_2D, depthbuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, fluidswidth, fluidsheight, 0, GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthbuffer, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);

	glGenTextures(1, &Velocity0);
	states_bindTexture(GL_TEXTURE_2D, Velocity0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, fluidswidth, fluidsheight, 0, GL_RGBA, GL_FLOAT, 0);
	//filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//set renderTexture as our color attachment#0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Velocity0, 0);

	glGenTextures(1, &Velocity1);
	states_bindTexture(GL_TEXTURE_2D, Velocity1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, fluidswidth, fluidsheight, 0, GL_RGBA, GL_FLOAT, 0);
	//filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//set renderTexture as our color attachment#1
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, Velocity1, 0);


	glGenTextures(1, &Pressure0);
	states_bindTexture(GL_TEXTURE_2D, Pressure0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, fluidswidth, fluidsheight, 0, GL_RGBA, GL_FLOAT, 0);
	//filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//set renderTexture as our color attachment#0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, Pressure0, 0);

	glGenTextures(1, &Pressure1);
	states_bindTexture(GL_TEXTURE_2D, Pressure1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, fluidswidth, fluidsheight, 0, GL_RGBA, GL_FLOAT, 0);
	//filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//set renderTexture as our color attachment#0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, Pressure1, 0);


	glGenTextures(1, &divergence);
	states_bindTexture(GL_TEXTURE_2D, divergence);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, fluidswidth, fluidsheight, 0, GL_RGBA, GL_FLOAT, 0);
	//filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//set renderTexture as our color attachment#0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, divergence, 0);


	//now set the list of draw buffers
	drawBuffers[0] = GL_COLOR_ATTACHMENT0;
	drawBuffers[1] = GL_COLOR_ATTACHMENT1;
	drawBuffers[2] = GL_COLOR_ATTACHMENT2;
	drawBuffers[3] = GL_COLOR_ATTACHMENT3;
	drawBuffers[4] = GL_COLOR_ATTACHMENT4;
	glDrawBuffers(5, drawBuffers);//this is finally where we tell the driver to draw to this paricular framebuffer

	//in case something goes wrong : 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error loading divergence FBO: incomplete\n");
		return -1;
	}

	glBindFramebuffer(GL_FRAMEBUFFER,0);

	//Jacobi_iter_FBO_1  : this FBO will be bound for stages 5 and 6
	glGenFramebuffers(1, &Jacobi_iter_FBO_1);
	glBindFramebuffer(GL_FRAMEBUFFER, Jacobi_iter_FBO_1);

	states_bindTexture(GL_TEXTURE_2D, divergence);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, divergence, 0);

	states_bindTexture(GL_TEXTURE_2D, Pressure0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Pressure0, 0);

	states_bindTexture(GL_TEXTURE_2D, Pressure1);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, Pressure1, 0);

	GLenum Jacobi_iter_1[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, Jacobi_iter_1);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error loading Jacobi_iter_FBO_1: incomplete\n");
		return -1;
	}


	//Jacobi_iter_FBO_2  : this FBO will be bound for stages 5 and 6
	glGenFramebuffers(1, &Jacobi_iter_FBO_2);
	glBindFramebuffer(GL_FRAMEBUFFER, Jacobi_iter_FBO_2);

	states_bindTexture(GL_TEXTURE_2D, divergence);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, divergence, 0);

	states_bindTexture(GL_TEXTURE_2D, Pressure1);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, Pressure1, 0);

	states_bindTexture(GL_TEXTURE_2D, Pressure0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, Pressure0, 0);

	GLenum Jacobi_iter_2[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, Jacobi_iter_2);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error loading Jacobi_iter_FBO_2: incomplete\n");
		return -1;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// The fullscreen quad's VBO
	/*static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};*/
	static const GLfloat g_quad_vertex_buffer_data[] = {
		-0.5f, -1.0f, 0.0f,
		1.0f, -0.5f, 0.0f,
		-1.0f, 1.0f, 0.5f,
		-0.5f, 1.0f, 0.0f,
		1.0f, -0.5f, 0.0f,
		1.0f, 1.0f, 0.5f,
	};

	glGenBuffers(1, &quad_vertexbuffer);
	states_bindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);//this is default : draw to screen
	
	return 0;
}

void fluids_simulate() {
	// 1: Force addition
	states_useProgram(addForce.programid);

	// TODO: Fill in addForce uniforms
	// glUniform2f(addForce.univec20, [placeholder mouseX], [placeholder mouseY]);
	// glUniform2f(addForce.univec21, [placeholder mouseDX], [placeholder mouseDY]);
	states_bindActiveTexture(0, GL_TEXTURE_2D, Velocity0);	//add V0 as input texture
	glUniform1i(addForce.texturepos[0], 0);
	states_bindVertexArray(inside);

	//2nd draw call
	glDrawArrays(GL_TRIANGLES, 0, 6);
	states_bindVertexArray(0);
	states_bindTexture(GL_TEXTURE_2D, 0);


	// 2: Advection
	states_useProgram(advectVelocity.programid);
	states_bindActiveTexture(0, GL_TEXTURE_2D, Velocity1);
	glUniform1i(advectVelocity.texturepos[1], 0);

	states_bindActiveTexture(1, GL_TEXTURE_2D, Velocity1);
	glUniform1i(advectVelocity.texturepos[0], 1);
	states_bindVertexArray(inside);
	
	//1st draw call
	glDrawArrays(GL_TRIANGLES,0,6);
	states_bindVertexArray(0);//unbind VAO
	states_bindTexture(GL_TEXTURE_2D, 0);
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// 3: Velocity boundary
	states_bindVertexArray(boundary);
		
	states_useProgram(velocityBoundary.programid);
	states_bindActiveTexture(0, GL_TEXTURE_2D, Velocity1);
	glUniform1i(velocityBoundary.texturepos[1],0);
	states_bindVertexArray(boundary);

	glDrawArrays(GL_TRIANGLES, 0, 24);
	states_bindVertexArray(0);//unbind VAO
	states_bindTexture(GL_TEXTURE_2D, 0);


	// 4: Divergence
	states_useProgram(divergenceShader.programid);
	states_bindActiveTexture(0, GL_TEXTURE_2D, Velocity0);
	glUniform1i(divergenceShader.texturepos[0], 0);
	states_bindVertexArray(inside);
	//4th draw call
	glDrawArrays(GL_TRIANGLES, 0, 6);
	states_bindVertexArray(0);
	states_bindTexture(GL_TEXTURE_2D, 0);


	// 5: Pressure computation (Jacobi)
	GLuint tempFBO = Jacobi_iter_FBO_1;
	GLuint tempPressure = Pressure0;
	int i;
	for (i = 0; i < 10; i++) { 
		glBindFramebuffer(GL_FRAMEBUFFER, tempFBO);
		states_useProgram(jacobiSolver.programid);

		states_bindActiveTexture(0, GL_TEXTURE_2D, divergence);
		glUniform1i(jacobiSolver.texturepos[0], 0);

		states_bindActiveTexture(1, GL_TEXTURE_2D, tempPressure);
		glUniform1i(jacobiSolver.texturepos[1], 1);

		states_bindVertexArray(inside);

		glDrawArrays(GL_TRIANGLES, 0, 6);


		// 6: Pressure boundary (Jacobi)
		states_useProgram(jacobiSolverBound.programid);
		states_bindVertexArray(boundary);

		states_bindActiveTexture(0, GL_TEXTURE_2D, Pressure0);
		glUniform1i(jacobiSolverBound.texturepos[0], 0);

		glDrawArrays(GL_TRIANGLES, 0, 24);
		states_bindVertexArray(0);
		states_bindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//swap pressure textures
		tempPressure = Pressure1;
		Pressure1 = Pressure0;
		Pressure0 = tempPressure;

		//swap FBOs
		tempFBO = Jacobi_iter_FBO_2;
		Jacobi_iter_FBO_2 = Jacobi_iter_FBO_1;
		Jacobi_iter_FBO_1 = tempFBO;
	}


	// 7: Subtract pressure gradient
	glBindFramebuffer(GL_FRAMEBUFFER, MainFBO);

	states_useProgram(subtractPressure.programid);
	states_bindActiveTexture(0, GL_TEXTURE_2D, Pressure0);
	glUniform1i(subtractPressure.texturepos[0], 0);

	states_bindActiveTexture(1, GL_TEXTURE_2D, Velocity0);
	glUniform1i(subtractPressure.texturepos[1], 1);

	states_bindVertexArray(inside);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	states_bindVertexArray(0);
	states_bindTexture(GL_TEXTURE_2D, 0);


	// 8: Copy v1 to v0
	states_useProgram(texCopyShader.programid);
	states_bindActiveTexture(0, GL_TEXTURE_2D, Velocity1);
	glUniform1i(texCopyShader.texturepos[1], 0);


	states_bindVertexArray(inside);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	states_bindVertexArray(0);

	states_bindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Draw to screen
	//By now we have successfully rendered to our texture. We will now draw on screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//use our quad shader
	states_useProgram(quadProgram.programid);

	//Bind out texture in texture unit #0
	states_bindActiveTexture(0, GL_TEXTURE_2D, Velocity0);
	glUniform1i(quadProgram.texturepos[0], 0);

	//states_bindActiveTexture(1, GL_TEXTURE_2D, Pressure0);
	//glUniform1i(quadProgram.texturepos[2], 1);

	//1st attribute : quad vertices
	glEnableVertexAttribArray(0);	//note that this corresponds to the layout=0 in shader
	states_bindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		0					// array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}
