#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Game.h"
#include "Camera.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// run without console window
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

Camera cam(glm::vec3(0.0f, 0.0f, 20.0f));

glm::vec3 camPos(0.0f, -3.0f, 2.0f);
glm::vec3 target(0.0f, 1.0f, 0.0f);
glm::vec3 up(0.0f, 0.0f, 1.0f);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void processInput(GLFWwindow*, Game&);
void framebuffer_size_callback(GLFWwindow*, int, int);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Online Bowling Game", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);
	
	Game game;
	float fsize = 1.0f;
	// main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// processing input
		processInput(window, game);
		

		// update game state
		game.Update(deltaTime);

		// rendering
		ImGui::Render();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// DoCamera
		glm::vec2 ballPos = game.ball.getPosition();
		//glm::mat4 view = glm::lookAt(camPos, target, up);
		glm::mat4 view = glm::lookAt(
			glm::vec3(ballPos, 0.0f) + glm::vec3(0.0f, -2.0f, 2.0f),
			glm::vec3(ballPos, 0.0f) + glm::vec3(0.0f, 2.0f, 0.0f),
			up
		);
		//game.setCamera(cam.GetViewMatrix());
		game.setCamera(view);
		
		// Game Rendering
		//game.setFloorLength(fsize);
		game.Render();

		// UI Rendering
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window, Game& game)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	auto oldPos = game.ball.getPosition();

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		auto newPos = oldPos - glm::vec2(deltaTime, 0.0f);
		if (newPos.x < -0.5f)
		{
			game.ball.setPosition(glm::vec2(-0.5, newPos.y));
		}
		else
		{
			game.ball.setPosition(newPos);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		auto newPos = oldPos + glm::vec2(deltaTime, 0.0f);
		if (newPos.x > 0.5f)
		{
			game.ball.setPosition(glm::vec2(0.5, newPos.y));
		}
		else
		{
			game.ball.setPosition(newPos);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		game.applyImpulse(glm::vec2(0.0f, 8.0f));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}