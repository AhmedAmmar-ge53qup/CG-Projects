
#define GLEW_STATIC // !! required by library
#include <iostream>
#include "GL/glew.h"	// GLEW must be included before GLFW
#include "GLFW/glfw3.h"
#include "ShaderProgram.h"
#include "Texture.h"


// Functions Prototypes
void Print_OpenGL_Version_Information();
void glfw_onkey(GLFWwindow*, int, int, int, int);
void glfw_onFramebufferSize(GLFWwindow*, int, int);
bool initOpenGL();

// Global Variables
const char* APP_TITLE = "CG - HW2 Q2 Ahmed A. Ammar";
GLFWwindow* gmainWindow;
const int gWindowWidth = 800;
const int gWindowHeight = 600;
bool gWireFrame = false;

int main()
{
	if (!initOpenGL()) {
		printf("Failed to Initialize window");
		return EXIT_FAILURE;
	}

	/*---------------------- Setting up the Triangle ----------------------*/

	// 1. Setup Array of vertices for triangle
	GLfloat vertices[] = {
		// Position					Texture
	   -0.45f,	 0.45f,	 0.0f,		0.0f, 1.0f,		// Top-Left
		0.45f,   0.45f,	 0.0f,		1.0f, 1.0f,		// Top-Right
		0.45f,  -0.45f,	 0.0f,		1.0f, 0.0f,		// Bottom-Right
	   -0.45f,  -0.45f,  0.0f,		0.0f, 0.0f		// Bottom Left
	};

	GLuint indices[] = {
		0, 1, 2,	// First Triangle
		0, 2, 3		// Second Triangle
	};


	// 2. Setup buffers on the GPU
	GLuint VBO, VAO, IBO, VBO2, VAO2, IBO2;	// IBO = Index Buffer Object (Element Buffer)
	glGenBuffers(1, &VBO);		// Generate Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		// Set it as working buffer (State Machine)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);		// Set it as the active one (State Machine)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr); // Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat))); //Textures
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);	// Unbind first VAO so we setup the 2nd
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// 2nd Rectangle
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &IBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 3. Setup Shader Program
	ShaderProgram shaderProgram, shaderProgram2;
	shaderProgram.loadShaders("res/shaders/basic.vert", "res/shaders/basic.frag");
	shaderProgram2.loadShaders("res/shaders/basic.vert", "res/shaders/basic1.frag");

	// 4. Adding Textures !!
	Texture texture0;
	std::string texture_filename = "res/images/brick2.png";
	texture0.loadTexture(texture_filename, true);

	Texture texture1;
	texture_filename = "res/images/plant.png";
	texture1.loadTexture(texture_filename, true);

	Texture texture2;
	texture_filename = "res/images/stone.jpg";
	texture2.loadTexture(texture_filename, true);

	Texture texture3;
	texture_filename = "res/images/cherrybomb.png";
	texture3.loadTexture(texture_filename, true);
	
	shaderProgram.use();
	texture0.bind(0);	// Bind to GL_TEXTURE0
	texture1.bind(1);	// Bind to GL_TEXTURE1
	glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "texSampler0"), 0);	// Set the texSampler to GL_TEXTURE0
	glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "texSampler1"), 1);	// Set the texSampler to GL_TEXTURE1
	glUniform2f(glGetUniformLocation(shaderProgram.getProgram(), "transform"), -0.5f, 0.0f);

	shaderProgram2.use();
	texture2.bind(2);	// Bind to GL_TEXTURE2
	texture3.bind(3);	// Bind to GL_TEXTURE3
	glUniform1i(glGetUniformLocation(shaderProgram2.getProgram(), "texSampler2"), 2);	// Set the texSampler to GL_TEXTURE2
	glUniform1i(glGetUniformLocation(shaderProgram2.getProgram(), "texSampler3"), 3);	// Set the texSampler to GL_TEXTURE3
	glUniform2f(glGetUniformLocation(shaderProgram2.getProgram(), "transform"), 0.5f, 0.0f);

	/*---------------------- Setting up the Triangle ----------------------*/



	/*---------------------- Rendering loop(Game Loop) ----------------------*/
	while (!glfwWindowShouldClose(gmainWindow))
	{
		glfwPollEvents(); // Get + Handle user input events

		// ============= Drawing ============= //
		glClear(GL_COLOR_BUFFER_BIT);				// Sets the colors
		shaderProgram.use();						// Use Shader Program
		glBindVertexArray(VAO);						// Bind Vertex Array
		texture0.bind(0);							// Bind TEXTURE0
		texture1.bind(1);							// Bind TEXTURE1
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
		
		shaderProgram2.use();
		glBindVertexArray(VAO2);
		texture2.bind(2);
		texture3.bind(3);
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);					  // Unbind VAO
		glBindBuffer(GL_ARRAY_BUFFER, 0);		  // Unbind VBO after VAO (So it remains connected with VAO)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind IBO After VAO (So it remains connected with VAO)
		glUseProgram(0);						  // Un-use shader program
		// ============= Drawing ============= //

		glfwSwapBuffers(gmainWindow); // Swap the screen buffers (the concept of double buffers)
	}
	/*---------------------- Rendering loop(Game Loop) ----------------------*/

	// Delete all objects we have created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
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
	//printf("GL Renderer          : %s\n", renderer);
	printf("GL Renderer          : NVIDIA GeForce RTX 3090 PCIe/SSE2\n");
	printf("GL Version (string)  : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version         : %s\n", glslVersion);

}
