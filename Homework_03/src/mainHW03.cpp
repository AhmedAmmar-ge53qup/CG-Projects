
#define GLEW_STATIC // !! required by library
#include <iostream>
#include "GL/glew.h"	// GLEW must be included before GLFW
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

// Functions Prototypes
void Print_OpenGL_Version_Information();
void glfw_onkey(GLFWwindow*, int, int, int, int);
void glfw_onFramebufferSize(GLFWwindow*, int, int);
bool initOpenGL();

// Global Variables
const char* APP_TITLE = "CG HW03 Ahmed A. Ammar";
GLFWwindow* gmainWindow;
int gWindowWidth = 800;
int gWindowHeight = 600;
bool gWireFrame = false;

// Experiment with translation
bool transDirection = true;
float offset = 0.0f;
float maxOffset = 0.7f;
float increment = 0.01f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.6f;
float minSize = 0.1f;

int main()
{

	if (!initOpenGL()) {
		printf("Failed to Initialize window");
		return EXIT_FAILURE;
	}

	/*---------------------- Setting up the Triangle ----------------------*/

	// 1. Setup Array of vertices for triangle
	GLfloat vertices[] = {
		// position		 // tex coords

	    //front face
	    -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
	    -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
	    -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f,

		  //back face
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

		  //left face
		 -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		 -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		 -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,

		  //right face
		  1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
		  1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f,
		  1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f,

		   //top face
		 -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
		  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
		  1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
		 -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
		 -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
		  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,

		   //bottom face
		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		  1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f
	};

	

	// 2. Setup buffers on the GPU
	GLuint VBO, VAO;	// IBO = Index Buffer Object (Element Buffer)
	glGenBuffers(1, &VBO);		// Generate Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		// Set it as working buffer (State Machine)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);		// Set it as the active one (State Machine)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr); // Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat))); //Textures
	glEnableVertexAttribArray(1);


	// 3. Setup Shader Program
	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("res/shaders/basic.vert", "res/shaders/basic.frag");
	shaderProgram.use();


	/*---------------------- Setting up the Triangle ----------------------*/


	double lastTime = glfwGetTime();
	float cubeAngle = 0.0f;
	/*---------------------- Rendering loop(Game Loop) ----------------------*/
	while (!glfwWindowShouldClose(gmainWindow))
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		glfwPollEvents(); // Get + Handle user input events

		// ============= Drawing ============= //
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Sets the colors
		shaderProgram.use();						// Use Shader Program
		glBindVertexArray(VAO);						// Bind Vertex Array

		// Model, View, Projection MVP
		glm::mat4 model(1.0), view(1.0), projection(1.0);
		glm::vec3 camPos(0.0f, 0.0f, 0.0f);
		glm::vec3 targetPos(0.0f, 0.0f, -1.0f);
		glm::vec3 up(0.0f, 1.0f, 0.0f);

		// Create Model Matrix
		cubeAngle += (GLfloat)(deltaTime * 50.0f);
		if (cubeAngle >= 360.0f) 
			cubeAngle = 0.0f;

		// Create View Matrix
		view = glm::lookAt(camPos, camPos + targetPos, up);

		//Create Projection Matrix
		projection = glm::perspective(glm::radians(45.0f), (GLfloat)gWindowWidth/(GLfloat)gWindowHeight, 0.1f, 100.0f);
		
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);
		

		float hzOffset = 0.7f; // Horizontal Offset Between Cubes
		float vrOffset = 1.5f; // Vertical Offset Between Cubes
		glm::vec3 scaleRatio = glm::vec3(0.2f, 0.2f, 0.2f);
		for (int j = 1; j <= 5; j++) {
			for (int i = 1; i <= 5; i++) {
				model = glm::mat4(1.0);	// Reset Model Matrix
				model = glm::translate(model, glm::vec3(vrOffset, 2.1f - (i * hzOffset), -5.0f)) *
					glm::scale(model, scaleRatio) *
					glm::rotate(model, glm::radians(cubeAngle), glm::vec3(1.0f*i, 1.0f*j, 0.0f));

				shaderProgram.setUniform("model", model);
				glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(GLuint));
			}
			vrOffset -= 0.75;
			scaleRatio -= glm::vec3(0.05f, 0.05f, 0.05f);
		}
	

		//for (int i = 1; i <= 5; i++) {
		//	model = glm::mat4(1.0);	// Reset Model Matrix
		//	model = glm::translate(model, glm::vec3(2.0f, 2.1f - (i * hzOffset), -5.0f)) *
		//		glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f)) *
		//		glm::rotate(model, glm::radians(cubeAngle + 20.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		//	shaderProgram.setUniform("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(GLuint));
		//}
		//for (int i = 1; i <= 5; i++) {
		//	model = glm::mat4(1.0);	// Reset Model Matrix
		//	model = glm::translate(model, glm::vec3(1.0f, 2.1f - (i * hzOffset), -5.0f)) *
		//		glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f)) *
		//		glm::rotate(model, glm::radians(cubeAngle + 20.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		//	shaderProgram.setUniform("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(GLuint));
		//}
		//for (int i = 1; i <= 5; i++) {
		//	model = glm::mat4(1.0);	// Reset Model Matrix
		//	model = glm::translate(model, glm::vec3(0.0f, 2.1f - (i * hzOffset), -5.0f)) *
		//		glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)) *
		//		glm::rotate(model, glm::radians(cubeAngle + 20.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		//	shaderProgram.setUniform("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(GLuint));
		//}
		//for (int i = 1; i <= 5; i++) {
		//	model = glm::mat4(1.0);	// Reset Model Matrix
		//	model = glm::translate(model, glm::vec3(-1.0f, 2.1f - (i * hzOffset), -5.0f)) *
		//		glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f)) *
		//		glm::rotate(model, glm::radians(cubeAngle + 20.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		//	shaderProgram.setUniform("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(GLuint));
		//}

		glBindVertexArray(0);					  // Unbind VAO
		glBindBuffer(GL_ARRAY_BUFFER, 0);		  // Unbind VBO after VAO (So it remains connected with VAO)
		glUseProgram(0);						  // Un-use shader program
		// ============= Drawing ============= //

		lastTime = currentTime;
		glfwSwapInterval(1);
		glfwSwapBuffers(gmainWindow); // Swap the screen buffers (the concept of double buffers)
	}
	/*---------------------- Rendering loop(Game Loop) ----------------------*/

	// Delete all objects we have created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// Delete window before ending the progrem
	glfwDestroyWindow(gmainWindow);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return EXIT_SUCCESS; // return 0;

}  // ==================== End Main =======================



bool initOpenGL() {

	// Initialize GLFW
	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return EXIT_FAILURE;
	}

	// Setup GLFW properties and set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// Core Profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// Allow Forward Compatibility

	// Creating main window & Obtaining a pointer to it
	gmainWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	if (gmainWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Set Context for GLEW with mainWindow, cuz we might have multiple windows !!
	glfwMakeContextCurrent(gmainWindow);

	// Initialize GLEW
	//glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE; // return -1;
	}

	// Set the callback function (will be called upon event to handle it);
	glfwSetKeyCallback(gmainWindow, glfw_onkey);

	// Handle Resizing the Window
	glfwSetFramebufferSizeCallback(gmainWindow, glfw_onFramebufferSize);

	// Enable Blending
	glEnable(GL_BLEND);

	// Clear the colorBuffer to the following values
	glClearColor(0.2f, 0.4f, 0.6f, 1.0f); // Set the background color (state machine function)
	
	glEnable(GL_DEPTH_TEST);

	// Print OpenGL Version
	Print_OpenGL_Version_Information();
	return true;
}

// Exit Callback Function // Handle Keyboard
void glfw_onkey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		gWireFrame = !gWireFrame;	// Toggling
		if (gWireFrame == true)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


// Handle resizing the window
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height) {
	gWindowWidth = width;
	gWindowHeight = height;
	glViewport(0, 0, width, height);
}



// Print OpenGL version information
void Print_OpenGL_Version_Information()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL Vendor            : %s\n", vendor);
	printf("GL Renderer          : %s\n", renderer);
	//printf("GL Renderer          : NVIDIA GeForce RTX 3090 PCIe/SSE2\n");
	printf("GL Version (string)  : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version         : %s\n", glslVersion);

}
