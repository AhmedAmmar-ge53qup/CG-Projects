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
#include "imgui/ImGuiFileDialog.h"

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

glm::vec3 getMouseRay(double, double);

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
	static int numModels = 4;
	std::vector<Model> models(4);
	models[0].loadModel("res/models/floor.obj");
	models[1].loadModel("res/models/woodcrate.obj");
	models[2].loadModel("res/models/backpack/backpack.obj");
	models[3].loadModel("res/models/cylinder.obj");

	static std::vector<float[3]> modelPos(50);
	static std::vector<float[3]> modelScale(50);
	for (int i = 0; i < 50; i++)
		for (int j = 0; j < 3; j++)
		{
			if (i == 0 && (j == 0 || j == 2)) {
				modelScale[i][j] = 10.0f;
				continue;
			}
			modelScale[i][j] = 1.0f;
		}
	static std::vector<float[3]> modelRotate(50);

	//-------------------------------------------------------------- THE CUBES -----------------------------------------------------------------
	// Loading the Cubes
	static int numCubes = 1;			// Must start with at least 1 cube on screen
	std::vector<Cube> cubes(1);	// Constructor Gets Called here automatically
	static std::vector<float[3]> cubePositions(50);	// 50 cubes are the maximum limit
	static std::vector<float[3]> cubeScales(50);
	for (int i = 0; i < 50; i++)
		for (int j = 0; j < 3; j++) 
			cubeScales[i][j] = 1.0f;
	static std::vector<float[3]> cubeRotations(50);
	static std::vector<float[3]> cubeColors(50);
	for (int i = 0; i < 50; i++)
		for (int j = 0; j < 3; j++)
			cubeColors[i][j] = 0.29f;


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

			// open Dialog Simple
			if (ImGui::Button("Open File Dialog"))
				ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".obj,.cpp,.h,.hpp", ".");

			// display
			if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
			{
				// action if OK
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
					std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
					std::replace(filePathName.begin(), filePathName.end(), '\\', '/');

					models.push_back(Model());
					models[numModels++].loadModel(filePathName);
				}

				// close
				ImGuiFileDialog::Instance()->Close();
			}

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

		ImGui::Begin("Imported Models");
		for (int i = 0; i < numModels; i++)
		{
			std::string s = models[i].getFileName();
			s.insert(0, "--------------- ");
			s.insert(s.length(), " ---------------");
			ImGui::Text(s.c_str());

			s = "Position";
			s.insert(s.length(), std::to_string(i));
			ImGui::SliderFloat3(s.c_str(), modelPos[i], -10.0f, 10.0f);
			if (ImGui::Button(s.c_str()))
				for (int j = 0; j < 3; j++)
					modelPos[i][j] = 0.0f;

			s = "Scale";
			s.insert(s.length(), std::to_string(i));
			ImGui::SliderFloat3(s.c_str(), modelScale[i], -10.0f, 10.0f);
			if (ImGui::Button(s.c_str()))
				for (int j = 0; j < 3; j++) {
					if (i == 0 && (j == 0 || j == 2)) {
						modelScale[i][j] = 10.0f;
						continue;
					}
					modelScale[i][j] = 1.0f;
				}

			s = "Rotate";
			s.insert(s.length(), std::to_string(i));
			ImGui::SliderFloat3(s.c_str(), modelRotate[i], 0.0f, 360.0f);
			if (ImGui::Button(s.c_str()))
				for (int j = 0; j < 3; j++)
					modelRotate[i][j] = 0.0f;

			model = glm::translate(glm::mat4(1.0), glm::vec3(modelPos[i][0], modelPos[i][1], modelPos[i][2]));
			model = glm::rotate(model, glm::radians(modelRotate[i][0]), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(modelRotate[i][1]), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(modelRotate[i][2]), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(modelScale[i][0], modelScale[i][1], modelScale[i][2]));
			shaderProgram.setUniform("model", model);
			models[i].Draw(shaderProgram);	// glBindVertexArray(0) called automatically for cleanup
		}
		ImGui::End();

		cubeShader.use();
		cubeShader.setUniform("model", glm::mat4(1.0));
		cubeShader.setUniform("view", view);
		cubeShader.setUniform("projection", projection);

		for (int i = 0; i < numCubes; i++) {
			std::string s = "Cube " + std::to_string(i + 1);
			ImGui::Begin(s.c_str());
			ImGui::SliderFloat3("Position", cubePositions[i], -10.0f, 10.0f);
			if (ImGui::Button("Position"))
				for (int j = 0; j < 3; j++)
					cubePositions[i][j] = 0.0f;

			ImGui::SliderFloat3("Scale", cubeScales[i], -10.0f, 10.0f);
			if (ImGui::Button("Scale"))
				for (int j = 0; j < 3; j++)
					cubeScales[i][j] = 1.0f;

			ImGui::SliderFloat3("Rotate", cubeRotations[i], 0.0f, 360.0f);
			if (ImGui::Button("Rotate"))
				for (int j = 0; j < 3; j++)
					cubeRotations[i][j] = 0.0f;

			ImGui::ColorEdit3("Color", cubeColors[i]); // Edit 3 floats representing a color
			ImGui::End();

			cubeShader.setUniform("color", glm::vec3(cubeColors[i][0], cubeColors[i][1], cubeColors[i][2]));
			cubes[i].model = glm::translate(glm::mat4(1.0f), glm::vec3(cubePositions[i][0], cubePositions[i][1], cubePositions[i][2]));
			cubes[i].model = glm::scale(cubes[i].model, glm::vec3(cubeScales[i][0], cubeScales[i][1], cubeScales[i][2]));
			cubes[i].model = glm::rotate(cubes[i].model, glm::radians(cubeRotations[i][0]), glm::vec3(1.0f, 0.0f, 0.0f));
			cubes[i].model = glm::rotate(cubes[i].model, glm::radians(cubeRotations[i][1]), glm::vec3(0.0f, 1.0f, 0.0f));
			cubes[i].model = glm::rotate(cubes[i].model, glm::radians(cubeRotations[i][2]), glm::vec3(0.0f, 0.0f, 1.0f));
			cubes[i].Draw(cubeShader);
		}

		// Rendering ImGui
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(gWindow, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers
		glfwSwapBuffers(gWindow);

		lastTime = currentTime;
	} // End of Game Loop

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
	if (glfwGetKey(gWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;
		fov = glm::clamp(fov, 1.0, 120.0);
		fpsCamera.setFOV((float)fov);
	}

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

	std::cout << "[" << getMouseRay(mouseX, mouseY).x << ", " << getMouseRay(mouseX, mouseY).y << ", " << getMouseRay(mouseX, mouseY).z << "]\n";

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
		fpsCamera.rotate((float)(gWindowWidth / 2.0f - mouseX) * MOUSE_SENSITIVITY, (float)(gWindowHeight / 2.0f - mouseY) * MOUSE_SENSITIVITY);
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

glm::vec3 getMouseRay(double mouseX, double mouseY) {
	// Get Mouse Position in Normalized Device Coordinates
	float x = (2.0f * mouseX) / gWindowWidth - 1;
	float y = (2.0f * mouseY) / gWindowHeight - 1;
	glm::vec2 ndc = glm::vec2(x, y);

	// Get Mouse Clip Coordinates
	glm::vec4 clipCoords = glm::vec4(ndc.x, ndc.y, -1.0f, 1.0f);

	// Get Mouse Eye Coordinates
	glm::mat4 projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);
	glm::mat4 invertedProjection = glm::inverse(projection);
	glm::vec4 eyeCoords = clipCoords * invertedProjection;

	// Get Mouse in World Coordinates
	glm::mat4 invertedView = glm::inverse(fpsCamera.getViewMatrix());
	glm::vec4 rayWorld = eyeCoords * invertedView;
	glm::vec3 worldRay = glm::normalize(glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z));

	return worldRay;
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
