#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "headers/ShaderClass.hpp"
#include "headers/Texture.hpp"
#include "headers/Mesh.hpp"
#include "headers/imgui_handler.hpp"
#include "headers/app_state.hpp"
#include "headers/app_callbacks.hpp"
#include "headers/app_logic.hpp"

int main(void) {
	GLFWwindow* window = initGLFW();
	if (!window)
		return -1;

	imgui_init(window, "#version 330");

	Mesh mesh;
	Shader shaderProgram("src/shaders/default.vert", "src/shaders/default.frag");
	shaderProgram.Activate();

	GLuint textureID = Texture::createWhiteTexture();
	shaderProgram.setInt("ourTexture", 0);

	while (!glfwWindowShouldClose(window)) {
		// Input and events
		processInput(window);
		glfwPollEvents();

		// Update cursor state
		if (mouseControlEnabled) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		} else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		// Clear screen
		glClearColor(0.03f, 0.025f, 0.02f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handle model loading
		if (imgui_model_changed())
			loadModel(imgui_get_selected_model(), mesh, textureID);

		// Handle texture changes
		if (imgui_texture_changed() && meshLoaded)
			handleTextureChange(mesh, textureID);

		// Update auto-rotation
		if (autoRotateEnabled) {
			rotationAngle += 15.0f * deltaTime;
			if (rotationAngle > 360.0f)
				rotationAngle -= 360.0f;
		}

		// Set up transformation matrices
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), BASE_ASPECT_RATIO, 0.1f, 100.0f);

		// Render
		shaderProgram.Activate();
		shaderProgram.setMat4("model", model);
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);

		// Update flip texture state
		flipTexture = imgui_get_flip_texture();
		if (flipTexture) {
			GLint flipLoc = glGetUniformLocation(shaderProgram.ID, "flipTexture");
			glUniform1i(flipLoc, 1);
		} else {
			GLint flipLoc = glGetUniformLocation(shaderProgram.ID, "flipTexture");
			glUniform1i(flipLoc, 0);
		}

		if (meshLoaded)
			mesh.Draw(shaderProgram);

		// UI
		imgui_render_ui(windowWidth, windowHeight);
		imgui_draw();

		// Swap buffers and update timing
		glfwSwapBuffers(window);
		glfwSwapInterval(1);

		if (printFps)
			CalculateFrameRate();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	imgui_shutdown();
	glfwTerminate();
	return 0;
}

