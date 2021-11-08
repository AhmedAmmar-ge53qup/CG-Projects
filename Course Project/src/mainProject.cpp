//-----------------------------------------------------------------------------
// mainOBJModelLoading.cpp
//
// - Demo loading OBJ files
//-----------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <vector>
#define GLEW_STATIC
#include "GL/glew.h"	// Important - this header must come before glfw3 header
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //added for transform
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"
#include "Cube.h"

// Global Variables
const char* APP_TITLE = "CG HW4 Ahmed A. Ammar";
int gWindowWidth = 1200;
int gWindowHeight = 900;
GLFWwindow* gWindow = NULL;
bool gWireframe = false;
const std::string texture1Filename = "res/images/box.png";
const std::string texture2Filename = "res/images/mario1.png";
const std::string floorImage = "res/images/floor.png";

// experiment with translation
bool transDirection = true;
float offset = 0.0f;
float maxOffest = 0.7f;
float increment = 0.01f;

// experiment with rotation
float curAngle = 0.0f;

// experiment with scaling??
bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

//FPSCamera fpsCamera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(1.0, 1.0, 1.0));
FPSCamera fpsCamera(glm::vec3(0.0f, 2.5f, 10.0f));

const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 5.0; // units per second
const float MOUSE_SENSITIVITY = 0.1f;

// Function prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime);
void showFPS(GLFWwindow* window);
bool initOpenGL();
void Print_OpenGL_Version_Information();

//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
int main()
{
	if (!initOpenGL())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}
	//print card info
	Print_OpenGL_Version_Information();

	//setting shaders
	ShaderProgram shaderProgram, cubeShader;
	shaderProgram.loadShaders("res/shaders/main.vert", "res/shaders/main.frag");
	cubeShader.loadShaders("res/shaders/cube.vert", "res/shaders/cube.frag");

	// Loading the models
	const int numModels = 4;
	Model models[numModels];
	models[0].loadModel("res/models/cylinder.obj");
	models[1].loadModel("res/models/woodcrate.obj");
	models[2].loadModel("res/models/backpack/backpack.obj");
	models[3].loadModel("res/models/floor.obj");

	// Model positions
	glm::vec3 modelPos[] = {
		glm::vec3(-2.5f,1.4f, 0.0f),	// cylinder
		glm::vec3(2.5f, 1.0f, 0.0f),	// crate
		glm::vec3(0.0f, 4.0f, 0.0f),	// backpack
		glm::vec3(0.0f, 0.0f, 0.0f)		// floor
	};

	// Model scale
	glm::vec3 modelScale[] = {
		glm::vec3(0.7f, 0.7f, 0.7f),	// cylinder
		glm::vec3(1.0f, 1.0f, 1.0f),	// crate
		glm::vec3(1.0f, 1.0f, 1.0f),	// backpack
		glm::vec3(10.0f, 1.0f, 10.0f)	// floor
	};

	//-------------------------------------------------------------- THE CUBES -----------------------------------------------------------------
	// Loading the Cubes
	static int numCubes = 2;
	std::vector<Cube> cubes(numCubes);	// Constructor Gets Called here automatically
	static float cubePositions[2][3] = { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };


	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(gWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();

	// ImGui window variables
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	double lastTime = glfwGetTime();
	float cubeAngle = 0.0f;
	while (!glfwWindowShouldClose(gWindow))
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		showFPS(gWindow);
		
		// Poll for and process events
		glfwPollEvents();
		update(deltaTime);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// Render ImGui Window
		{
			static float f = 0.0f;
			static int counter = 0;
		

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat3("cube1", cubePositions[0], -1.0f, 1.0f);            // Edit 3 float using a slider from -1.0f to 1.0f
			ImGui::SliderFloat3("cube2", cubePositions[1], -1.0f, 1.0f);            // Edit 3 float using a slider from -1.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			{
				numCubes++;
				cubes.push_back(Cube());	// Add a new cube
			}
				
			ImGui::SameLine();
			ImGui::Text("counter = %d", numCubes);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0), view(1.0), projection(1.0);

		// Create the View matrix
		view = fpsCamera.getViewMatrix();

		// Create the projection matrix
		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);

		shaderProgram.use();

		// Pass the matrices to the shader
		shaderProgram.setUniform("model", model);
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);

		for (int i = 0; i < numModels; i++)
		{
			model = glm::translate(glm::mat4(1.0), modelPos[i]) * glm::scale(glm::mat4(1.0), modelScale[i]);
			if (i == 2)
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
			shaderProgram.setUniform("model", model);

			models[i].Draw(shaderProgram);	// glBindVertexArray(0) called automatically for cleanup
		}


		cubeShader.use();
		cubeShader.setUniform("model", glm::mat4(1.0));
		cubeShader.setUniform("view", view);
		cubeShader.setUniform("projection", projection);

		for (int i = 0; i < numCubes; i++) {
			std::string s = "Cube " + std::to_string(i + 1);
			ImGui::Begin(s.c_str());
			ImGui::SliderFloat3("Position", cubePositions[i], -1.0f, 1.0f);
			ImGui::End();

			cubes[i].model = glm::translate(glm::mat4(1.0f), glm::vec3(cubePositions[i][0], cubePositions[i][1], cubePositions[i][2]));
			cubes[i].Draw(cubeShader);
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(gWindow, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers
		glfwSwapBuffers(gWindow);

		lastTime = currentTime;
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}


//-----------------------------------------------------------------------------
// Initialize GLFW and OpenGL
//-----------------------------------------------------------------------------
bool initOpenGL()
{
	// Intialize GLFW 
	// GLFW is configured.  Must be called before calling any GLFW functions
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3

	// Create an OpenGL 3.3 core, forward compatible context window
	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	if (gWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(gWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	// Set the required callback functions
	glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
	glfwSetScrollCallback(gWindow, glfw_onMouseScroll);

	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);

	// Define the viewport dimensions
	glViewport(0, 0, gWindowWidth, gWindowHeight);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	return true;
}

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		gWireframe = !gWireframe;
		if (gWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
	gWindowWidth = width;
	gWindowHeight = height;
	glViewport(0, 0, width, height);
}

//-----------------------------------------------------------------------------
// Called by GLFW when the mouse wheel is rotated
//-----------------------------------------------------------------------------
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
{
	double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

	fov = glm::clamp(fov, 1.0, 120.0);

	fpsCamera.setFOV((float)fov);
}

bool firstClick = true;
//-----------------------------------------------------------------------------
// Update stuff every frame
//-----------------------------------------------------------------------------
void update(double elapsedTime)
{
	// Camera orientation
	double mouseX, mouseY;
		
	// Get the current mouse cursor position delta
	glfwGetCursorPos(gWindow, &mouseX, &mouseY);

	if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{	
		if (firstClick) {
			glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);
			firstClick = false;
			glfwGetCursorPos(gWindow, &mouseX, &mouseY);
		}
		glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

		glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Rotate the camera the difference in mouse distance from the center screen.  Multiply this delta by a speed scaler
		fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);
	}
	else if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
		


	// Camera FPS movement

	// Forward/backward
	if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
	else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());

	// Strafe left/right
	if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());
	else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());

	// Up/down
	if (glfwGetKey(gWindow, GLFW_KEY_E) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
	else if (glfwGetKey(gWindow, GLFW_KEY_Q) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());
}

//-----------------------------------------------------------------------------
// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the window caption bar.
//-----------------------------------------------------------------------------
void showFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

	elapsedSeconds = currentSeconds - previousSeconds;

	// Limit text updates to 4 times per second
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		// The C++ way of setting the window title
		std::ostringstream outs;
		outs.precision(3);	// decimal places
		outs << std::fixed
			<< APP_TITLE << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << msPerFrame << " (ms)";
		glfwSetWindowTitle(window, outs.str().c_str());

		// Reset for next average.
		frameCount = 0;
	}

	frameCount++;
}
// Print OpenGL version information
void Print_OpenGL_Version_Information()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion =
		glGetString(GL_SHADING_LANGUAGE_VERSION);

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("GL Vendor            : %s\n", vendor);
	printf("GL Renderer          : %s\n", renderer);
	printf("GL Version (string)  : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version         : %s\n", glslVersion);
}
