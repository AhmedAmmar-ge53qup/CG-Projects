//-----------------------------------------------------------------------------
// InitOpenGL_1.cpp //
//
// 1. Creates a basic window and OpenGL 3.3 context using GLFW.
// 2. Print the graphics card information & Shader version
// 3. Add keyboard input handling
//-----------------------------------------------------------------------------
#include <iostream>

#define GLEW_STATIC // !!
#include "GL/glew.h"	// GLEW must be included before GLFW
#include "GLFW/glfw3.h"

// Functions Prototypes
void Print_OpenGL_Version_Information();
void glfw_onkey(GLFWwindow*, int, int, int, int);
void glfw_onFramebufferSize(GLFWwindow*, int, int);
bool initOpenGL();

// Global Variables
const char* APP_TITLE = "Computer Graphics - Cleanup";
GLFWwindow* gmainWindow;
const int gWindowWidth = 800;
const int gWindowHeight = 600;




//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
int main()
{

	if (!initOpenGL()) {
		std::cerr << "Failed to Initalize Window" << std:: endl;
		return EXIT_FAILURE;
	}


	/*          Rendering loop(Game Loop)       */
	while (!glfwWindowShouldClose(gmainWindow))
	{
		// Get + Handle user input events
		glfwPollEvents();

		// ============= Drawing ============= //
		glClear(GL_COLOR_BUFFER_BIT);
		// ============= Drawing ============= //

		glfwSwapBuffers(gmainWindow); // Swap the screen buffers (the concept of double buffers)
	}

	// Clean up. Terminate GLFW, clearing any resources allocated by GLFW
	glfwTerminate();
	return EXIT_SUCCESS; // return 0;
}



bool initOpenGL() {

	// Initialize GLFW
	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return EXIT_FAILURE; // return -1;
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
		return -1;
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

	// Clear the colorBuffer
	glClearColor(0.2f, 0.4f, 0.6f, 1.0f); // Set the background color

	// Print OpenGL Version
	Print_OpenGL_Version_Information();			
	return true;
}



// Exit Callback Function // Handle Keyboard
void glfw_onkey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


// Handle resizing the window
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height) {
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
	printf("GL Version (string)  : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version         : %s\n", glslVersion);
}
