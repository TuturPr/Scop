#include "headers/app_state.hpp"
#include "headers/Camera.hpp"

// Camera and input state
bool firstMouse = true;
float lastX = 1280.0f / 2.0;
float lastY = 720.0 / 2.0;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Key press tracking
bool vKeyPressed = false;
bool mKeyPressed = false;
bool cKeyPressed = false;
bool rKeyPressed = false;

// Application state
bool wireframeMode = false;
bool mouseControlEnabled = false;
bool meshLoaded = false;
bool autoRotateEnabled = true;
float rotationAngle = 0.0f;

// Mesh state
glm::vec3 meshCenter(0.0f);
GLuint originalMTLTexture = 0;
std::string originalMTLTexturePath;
bool flipTexture = false;

// Window settings
float windowWidth = 1280;
float windowHeight = 720;
const float BASE_ASPECT_RATIO = 1280.0f / 720.0f;
const bool printFps = true;

// Camera instance
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
