#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Camera.hpp"

// Camera and input state
extern bool firstMouse;
extern float lastX;
extern float lastY;
extern float deltaTime;
extern float lastFrame;

// Key press tracking
extern bool vKeyPressed;
extern bool mKeyPressed;
extern bool cKeyPressed;
extern bool rKeyPressed;

// Application state
extern bool wireframeMode;
extern bool mouseControlEnabled;
extern bool meshLoaded;
extern bool autoRotateEnabled;
extern float rotationAngle;

// Mesh state
extern glm::vec3 meshCenter;
extern GLuint originalMTLTexture;
extern std::string originalMTLTexturePath;
extern bool flipTexture;

// Window settings
extern float windowWidth;
extern float windowHeight;
extern const float BASE_ASPECT_RATIO;
extern const bool printFps;

// Camera instance
extern Camera camera;
