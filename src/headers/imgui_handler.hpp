#ifndef IMGUI_HANDLER_HPP
#define IMGUI_HANDLER_HPP

#include <string>
#include <GLFW/glfw3.h>

void imgui_init(GLFWwindow* window, const char* glsl_version);
void imgui_render_ui(float& window_width, float& window_height);
void imgui_draw();
bool imgui_model_changed();
std::string imgui_get_selected_model();
bool imgui_texture_changed();
std::string imgui_get_texture_path();
void imgui_reset_texture_selection();
bool imgui_get_flip_texture();
void imgui_shutdown();

#endif
