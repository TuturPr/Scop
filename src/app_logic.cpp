#include "headers/app_logic.hpp"
#include "headers/app_state.hpp"
#include "headers/Texture.hpp"
#include "headers/ObjLoader.hpp"
#include "headers/ShaderClass.hpp"
#include "headers/imgui_handler.hpp"
#include "headers/Camera.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* initGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Scop", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return nullptr;
	}

	extern void framebuffer_size_callback(GLFWwindow*, int, int);
	extern void mouse_callback(GLFWwindow*, double, double);
	extern void scroll_callback(GLFWwindow*, double, double);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	glViewport(0, 0, (int)windowWidth, (int)windowHeight);
	glEnable(GL_DEPTH_TEST);

	return window;
}

void loadModel(const std::string& path, Mesh& mesh, GLuint& textureID)
{
	std::cout << "Loading model: " << path << "\n";
	try {
		ObjLoader loader;
		mesh = loader.load(path);
		meshLoaded = true;
		meshCenter = mesh.GetCenter();
		imgui_reset_texture_selection();

		camera.SetTarget(meshCenter);
		camera.SetDistance(5.0f);

		originalMTLTexture = 0;
		originalMTLTexturePath = "";

		if (!mesh.textures.empty() && !mesh.textures[0].path.empty()) {
			try {
				Texture tex(mesh.textures[0].path.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
				originalMTLTexture = tex.ID;
				originalMTLTexturePath = mesh.textures[0].path;
				textureID = originalMTLTexture;
				mesh.setTextureId(0, textureID);
				std::cout << "Loaded MTL texture: " << mesh.textures[0].path << "\n";
			} catch (const std::exception& e) {
				std::cout << "Failed to load MTL texture: " << e.what() << "\n";
				textureID = Texture::createGrayTexture();
				mesh.addTexture(textureID, "diffuse");
			}
		} else {
			textureID = Texture::createGrayTexture();
			mesh.addTexture(textureID, "diffuse");
			std::cout << "Using gray texture for model\n";
		}
	} catch (const std::exception& e) {
		std::cout << "Error loading model: " << e.what() << "\n";
	}
}

void handleTextureChange(Mesh& mesh, GLuint& textureID)
{
	std::string texture_path = imgui_get_texture_path();

	if (texture_path.empty()) {
		std::cout << "Reverting to original texture\n";
		if (originalMTLTexture != 0) {
			textureID = originalMTLTexture;
			mesh.setTextureId(0, textureID);
			std::cout << "Using MTL texture: " << originalMTLTexturePath << "\n";
		} else {
			textureID = Texture::createGrayTexture();
			mesh.setTextureId(0, textureID);
			std::cout << "Using gray texture\n";
		}
	} else {
		std::cout << "Loading texture: " << texture_path << "\n";
		try {
			bool isPNG = texture_path.find(".png") != std::string::npos || texture_path.find(".PNG") != std::string::npos;
			Texture tex(texture_path.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE, isPNG);
			textureID = tex.ID;
			mesh.setTextureId(0, textureID);
			std::cout << "Texture loaded successfully\n";
		} catch (const std::exception& e) {
			std::cout << "Failed to load texture: " << e.what() << ", reverting to MTL\n";
			if (originalMTLTexture != 0) {
				textureID = originalMTLTexture;
				mesh.setTextureId(0, textureID);
			} else {
				textureID = Texture::createGrayTexture();
				mesh.setTextureId(0, textureID);
			}
		}
	}
}
