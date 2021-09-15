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
const char* APP_TITLE = "Computer Graphics - Quad (2 Triangles)";
GLFWwindow* gmainWindow;
const int gWindowWidth = 800;
const int gWindowHeight = 600;
bool gWireFrame = false;

// Vertex Shader
const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"layout (location = 1) in vec3 color;"
"out vec3 vertColor;"
"void main()"
"{"
"	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"	vertColor = color;"
"}";

// Fragment Shader
const GLchar* fragmentShaderSource =
"#version 330 core\n"
"in vec3 vertColor;"
"out vec4 fragColor;" // Outputting fragColor to next step of pipeline"
"void main()"
"{"
"	fragColor = vec4(vertColor, 1.0f);" // Instead of hardcoding the colors in shader, as we did in 04_Triangle
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
	   -0.5f,	0.5f,	0.0f,	1.0f, 0.0f, 0.0f,	// Top-Left     Red  
		0.5f,   0.5f,	0.0f,	1.0f, 0.0f, 0.0f,	// Top-Right    Red
	    0.5f,  -0.5f,	0.0f,	0.0f, 1.0f, 0.0f,	// Bottom-Right Green
	   -0.5f,  -0.5f,   0.0f,	0.0f, 0.0f, 1.0f	// Bottom Left  Blue
	};

	GLuint indices[] = {
		0, 1, 2,	// First Triangle
		0, 2, 3		// Second Triangle
	};


	// 2. Setup buffers on the GPU
	GLuint VBO, VAO, IBO;	// IBO = Index Buffer Object (Element Buffer)
	glGenBuffers(1, &VBO);		// Generate Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		// Set it as working buffer (State Machine)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);		// Set it as the active one (State Machine)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), nullptr); // For Position, as u can see cuz its tightly packed, we can give total size of 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(sizeof(GL_FLOAT) * 3));
	glEnableVertexAttribArray(1);

	// 3. Setup Index Buffer
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// 4. Create Shaders
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	// 5. Compile Shaders & Return Shader Program
	GLuint shaderProgram = CompileShaders(vs, fs);

	/*---------------------- Setting up the Triangle ----------------------*/



	/*---------------------- Rendering loop(Game Loop) ----------------------*/
	while (!glfwWindowShouldClose(gmainWindow))
	{
		glfwPollEvents(); // Get + Handle user input events

		// ============= Drawing ============= //
		glClear(GL_COLOR_BUFFER_BIT);				// Sets the colors
		glUseProgram(shaderProgram);				// Bind Shader Program
		glBindVertexArray(VAO);						// Bind Vertex Array
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	// Bind Index Buffer

		//glDrawArrays(GL_TRIANGLES, 0, 6);	// Draws the triangle (6 = how many vertices we have)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);   // Unbind VAO
		glUseProgram(0);		// Un-use shader program
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind IBO
		// ============= Drawing ============= //

		glfwSwapBuffers(gmainWindow); // Swap the screen buffers (the concept of double buffers)
	}
	/*---------------------- Rendering loop(Game Loop) ----------------------*/

	// Delete all objects we have created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glDeleteProgram(shaderProgram);
	// Delete window before ending the progrem
	glfwDestroyWindow(gmainWindow);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return EXIT_SUCCESS; // return 0;

}  // ==================== End Main =======================



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
	printf("GL Renderer          : %s\n", renderer);
	printf("GL Version (string)  : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version         : %s\n", glslVersion);
}
