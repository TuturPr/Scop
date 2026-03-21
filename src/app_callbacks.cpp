#include "headers/app_callbacks.hpp"
#include "headers/app_state.hpp"
#include "headers/Camera.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	windowWidth = (float)width;
	windowHeight = (float)height;

	float currentAspectRatio = (float)width / (float)height;
	int viewportWidth, viewportHeight, viewportX = 0, viewportY = 0;

	if (currentAspectRatio > BASE_ASPECT_RATIO) {
		viewportHeight = height;
		viewportWidth = (int)(height * BASE_ASPECT_RATIO);
		viewportX = (width - viewportWidth) / 2;
	} else {
		viewportWidth = width;
		viewportHeight = (int)(width / BASE_ASPECT_RATIO);
		viewportY = (height - viewportHeight) / 2;
	}

	glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
	(void) window;
}

void CalculateFrameRate()
{
	static int frames = 0;
	static double lastTime = glfwGetTime();

	++frames;
	double currentTime = glfwGetTime();

	if (currentTime - lastTime >= 1.0)
	{
		std::cout << "Current Frames Per Second: " << frames << '\n';
		frames = 0;
		lastTime = currentTime;
	}
}

void handleToggleKey(int key, GLFWwindow* window, bool& keyPressed, std::function<void()> action)
{
	if (glfwGetKey(window, key) == GLFW_PRESS && !keyPressed)
	{
		action();
		keyPressed = true;
	}
	if (glfwGetKey(window, key) == GLFW_RELEASE)
		keyPressed = false;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	handleToggleKey(GLFW_KEY_V, window, vKeyPressed, [](){
		wireframeMode = !wireframeMode;
		glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
	});

	handleToggleKey(GLFW_KEY_M, window, mKeyPressed, [](){
		mouseControlEnabled = !mouseControlEnabled;
		if (mouseControlEnabled && meshLoaded) {
			camera.SetTarget(meshCenter);
			lastX = windowWidth / 2.0f;
			lastY = windowHeight / 2.0f;
			firstMouse = true;
		}
	});

	handleToggleKey(GLFW_KEY_C, window, cKeyPressed, [](){
		if (meshLoaded) {
			camera.SetTarget(meshCenter);
			std::cout << "Centered on object\n";
		}
	});

	handleToggleKey(GLFW_KEY_R, window, rKeyPressed, [](){
		autoRotateEnabled = !autoRotateEnabled;
		std::cout << (autoRotateEnabled ? "Auto-rotate enabled\n" : "Auto-rotate disabled\n");
	});

	// Camera movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (!mouseControlEnabled)
		return;

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
	(void)window;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
	(void)window;
	(void)xoffset;
}
