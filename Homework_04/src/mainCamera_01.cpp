#define GLEW_STATIC // !! required by library
#include <iostream>
#include "GL/glew.h"	// GLEW must be included before GLFW
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Camera.h"

// Functions Prototypes
void Print_OpenGL_Version_Information();
void glfw_onkey(GLFWwindow*, int, int, int, int);
void glfw_onMouseScroll(GLFWwindow*, double, double);
void update(double);
void glfw_onFramebufferSize(GLFWwindow*, int, int);
bool initOpenGL();

// Global Variables
const char* APP_TITLE = "Computer Graphics - Object Model Loading";
GLFWwindow* gmainWindow;
int gWindowWidth = 800;
int gWindowHeight = 600;
bool gWireFrame = false;
std::string texture1_filename = "res/images/brick2.png";
std::string texture2_filename = "res/images/mario.png";
std::string gridImage = "res/images/stone.jpg";

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

// Setting the FPS Camera
FPSCamera fpsCamera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f));
const double ZOOM_SENSITIVTY = -3.0f;
const float MOVE_SPEED = 5.0f;
const float MOUSE_SENSITIVTY = 0.1f;

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
	   -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
	   -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
	   -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		 //back face
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		  //left face
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		 -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		  //right face
		  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		   //top face
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
		  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

		   //bottom face
		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f
	};


	// 2. Setup buffers on the GPU
	GLuint VBO, VAO;	// IBO = Index Buffer Object (Element Buffer)
	glGenBuffers(1, &VBO);		// Generate Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		// Set it as working buffer (State Machine)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);		// Set it as the active one (State Machine)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr); // Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat))); //Textures
	glEnableVertexAttribArray(1);


	// 3. Setup Shader Program
	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("res/shaders/basic.vert", "res/shaders/basic.frag");

	// 4. Adding Textures !!
	Texture texture1;
	texture1.loadTexture(texture1_filename, true);

	Texture texture2;
	texture2.loadTexture(texture2_filename, true);

	Texture floorTexture;
	floorTexture.loadTexture(gridImage, true);
	
	shaderProgram.use();
	texture1.bind(0);	// Bind to GL_TEXTURE0
	texture2.bind(1);	// Bind to GL_TEXTURE1
	glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "texSampler1"), 0);	// Set the texSampler to GL_TEXTURE0
	glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "texSampler2"), 1);	// Set the texSampler to GL_TEXTURE

	/*---------------------- Setting up the Triangle ----------------------*/


	double lastTime = glfwGetTime();
	float cubeAngle = 0.0f;
	glm::vec3 cubePos(0.0f, 0.0f, 0.0f);
	glm::vec3 floorPos(0.0f, -1.0f, 0.0f);
	/*---------------------- Rendering loop(Game Loop) ----------------------*/
	while (!glfwWindowShouldClose(gmainWindow))
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;


		glfwPollEvents(); // Get + Handle user input events
		update(deltaTime);

		// ============= Drawing ============= //
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Sets the colors
		shaderProgram.use();						// Use Shader Program
		glBindVertexArray(VAO);						// Bind Vertex Array
		texture1.bind(0);							// Bind TEXTURE0
		texture2.bind(1);							// Bind TEXTURE1

		// Model, View, Projection MVP
		glm::mat4 model(1.0), view(1.0), projection(1.0);

		// Draw the Cube
		view = fpsCamera.getViewMatrix();
		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (GLfloat)gWindowWidth/(GLfloat)gWindowHeight, 0.1f, 100.0f);
		
		shaderProgram.setUniform("model", model);
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(GLuint));
		glBindVertexArray(0);
		
		// Draw the floor
		floorTexture.bind(0);
		model = glm::translate(model, floorPos) * glm::scale(model, glm::vec3(10.0f, 0.01f, 10.0f));
		shaderProgram.setUniform("model", model);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(GLuint));


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
	glfwSetScrollCallback(gmainWindow, glfw_onMouseScroll);

	// Set mouse to center & hide cursor
	glfwSetInputMode(gmainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(gmainWindow, gWindowWidth / 2.0f, gWindowHeight / 2.0f);

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

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		gWireFrame = !gWireFrame;	// Toggling
		if (gWireFrame == true)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

// Handle Mouse Scrolling
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY) 
{
	double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVTY;
	fov = glm::clamp(fov, 1.0, 120.0);
	fpsCamera.setFOV((float)fov);
}

// Update the Camera every frame
void update(double elapsedTime)
{
	double mouseX, mouseY;
	glfwGetCursorPos(gmainWindow, &mouseX, &mouseY);	// It will return the position to the variables mouseX, mouseY

	// Rotate the camera
	fpsCamera.rotate((float)(gWindowWidth / 2.0f - mouseX) * MOUSE_SENSITIVTY,
		(float)(gWindowHeight / 2.0f - mouseY) * MOUSE_SENSITIVTY);

	// Return camera to center
	glfwSetCursorPos(gmainWindow, gWindowWidth / 2, gWindowHeight / 2);

	// Handle Camera movement based on WASD
	if (glfwGetKey(gmainWindow, GLFW_KEY_W) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
	if (glfwGetKey(gmainWindow, GLFW_KEY_S) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());
	// Left and right
	if (glfwGetKey(gmainWindow, GLFW_KEY_A) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());
	if (glfwGetKey(gmainWindow, GLFW_KEY_D) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());

	// Up and down
	if (glfwGetKey(gmainWindow, GLFW_KEY_E) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
	if (glfwGetKey(gmainWindow, GLFW_KEY_Q) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());


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
