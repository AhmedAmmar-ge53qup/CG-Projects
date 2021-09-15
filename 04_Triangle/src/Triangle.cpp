#define GLEW_STATIC // !!
#include <iostream>
#include "GL/glew.h"	// GLEW must be included before GLFW
#include "GLFW/glfw3.h"

// Functions Prototypes
void Print_OpenGL_Version_Information();
void glfw_onkey(GLFWwindow*, int, int, int, int);
void glfw_onFramebufferSize(GLFWwindow*, int, int);
bool initOpenGL();
GLuint CompileShaders(GLuint, GLuint);

// Global Variables
const char* APP_TITLE = "Computer Graphics - Hello Triangle";
GLFWwindow* gmainWindow;
const int gWindowWidth = 800;
const int gWindowHeight = 600;

// Vertex Shader
const GLchar* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"void main()"
"{"
"	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"}";

// Fragment Shader
const GLchar* fragmentShaderSource =
"#version 330 core\n"
"out vec4 fragColor;"
"void main()"
"{"
"	fragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);"
"}";


int main()
{

	if (!initOpenGL()) {
		printf("Failed to Initialize window");
		return EXIT_FAILURE;
	}

	/*---------------------- Setting up the Triangle ----------------------*/

	// 1. Setup Array of vertices for triangle
	GLfloat vertices[] = {
		0.0f,	0.5f,	0.0f,	// Top
		0.5f,  -0.5f,	0.0f,	// Right
	   -0.5f,  -0.5f,	0.0f,	// Left
	};

	// 2. Setup buffers on the GPU
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);		// Generate Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		// Set it as working buffer (State Machine)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);		// Set it as the active one (State Machine)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), NULL); // 3*sizeof(GL_FLOAT) is = 12 bytes
	glEnableVertexAttribArray(0);


	// 3. Create Shaders
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	// 4. Compile Shaders & Return Shader Program
	GLuint shaderProgram = CompileShaders(vs, fs);	
	
	/*---------------------- Setting up the Triangle ----------------------*/



	/*---------------------- Rendering loop(Game Loop) ----------------------*/
	while (!glfwWindowShouldClose(gmainWindow))
	{
		glfwPollEvents(); // Get + Handle user input events

		// ============= Drawing ============= //
		glClear(GL_COLOR_BUFFER_BIT);	// Sets the colors
		glUseProgram(shaderProgram);	// Uses the specified shaders
		glBindVertexArray(VAO);			// Set it as the active one (State Machine)
		glDrawArrays(GL_TRIANGLES, 0, 3);	// Draws the triangle (3 = how many vertices we have)
		glBindVertexArray(0);  // Unbind VAO
		// ============= Drawing ============= //

		glfwSwapBuffers(gmainWindow); // Swap the screen buffers (the concept of double buffers)
	}
	/*---------------------- Rendering loop(Game Loop) ----------------------*/

	// Delete all objects we have created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Delete window before ending the progrem
	glfwDestroyWindow(gmainWindow);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return EXIT_SUCCESS; // return 0;
}



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

	// Clear the colorBuffer to the following values
	glClearColor(0.2f, 0.4f, 0.6f, 1.0f); // Set the background color

	// Print OpenGL Version
	Print_OpenGL_Version_Information();
	return true;
}


GLuint CompileShaders(GLuint vertexShader, GLuint fragmentShader) {

	// Setup Vertex Shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check for Vertex Shader Compilation Error
	GLint result;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		printf("Error: Vertex Shader Failed to Compile.\n%s\n", infoLog);  // Can also use , std::cerr
	}

	// Setup Fragment Shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check for Fragment Shader Compilation Error
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
		printf("Error: Fragment Shader Failed to Compile.\n%s\n", infoLog);
	}

	// Create Shader Program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);	// Attaching Vertex Shader to the Program
	glAttachShader(shaderProgram, fragmentShader);	// Attaching Fragment Shader to the Program

	glLinkProgram(shaderProgram);

	// Check for linking Errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		printf("Error: Shader Program Failed to Link.\n%s\n", infoLog);
	}

	// Clean up, because we don't need them anymore (already compiled & linked)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
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
