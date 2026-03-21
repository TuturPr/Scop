#include "../includes/imgui-1.92.6/imgui.h"
#include "../includes/imgui-1.92.6/backends/imgui_impl_glfw.h"
#include "../includes/imgui-1.92.6/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <vector>
#include <string>

static std::vector<std::string> available_models;
static int selected_model = -1;
static bool model_changed = false;
static std::string current_model_path;
static std::vector<std::string> available_textures;
static int selected_texture = -1;
static bool texture_changed = false;
static std::string new_texture_path;
static bool flip_texture = false;

namespace fs = std::filesystem;

void imgui_init(GLFWwindow* window, const char* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Scan models directory
	std::string models_path = "src/models";
	if (fs::exists(models_path)) {
		for (const auto& entry : fs::directory_iterator(models_path)) {
			if (entry.path().extension() == ".obj") {
				available_models.push_back(entry.path().filename().string());
			}
		}
	}

	// Scan images directory
	std::string images_path = "images";
	if (fs::exists(images_path)) {
		for (const auto& entry : fs::directory_iterator(images_path)) {
			std::string ext = entry.path().extension().string();
			if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
				available_textures.push_back(entry.path().filename().string());
			}
		}
	}
}

void imgui_render_ui(float& window_width, float& window_height)
{
	(void)window_width;
	(void)window_height;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Main menu bar
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::BeginMenu("Open Model")) {
				for (size_t i = 0; i < available_models.size(); i++) {
					if (ImGui::Selectable(available_models[i].c_str(), selected_model == (int)i)) {
						if (selected_model != (int)i) {
							selected_model = (int)i;
							model_changed = true;
							current_model_path = "src/models/" + available_models[i];
						}
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// Info window
	ImGui::SetNextWindowPos(ImVec2(10, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
	ImGui::Begin("Model Info");
	if (selected_model == -1) {
		ImGui::Text("Current Model: None");
	} else {
		ImGui::Text("Current Model: %s", available_models[selected_model].c_str());
	}
	ImGui::Text("Available Models: %zu", available_models.size());

	ImGui::Separator();
	ImGui::Text("Texture:");
	if (ImGui::BeginCombo("##texture_combo", selected_texture == -1 ? "None" : available_textures[selected_texture].c_str())) {
		if (ImGui::Selectable("None", selected_texture == -1)) {
			if (selected_texture != -1) {
				selected_texture = -1;
				new_texture_path = "";
				texture_changed = true;
			}
		}
		for (size_t i = 0; i < available_textures.size(); i++) {
			if (ImGui::Selectable(available_textures[i].c_str(), selected_texture == (int)i)) {
				if (selected_texture != (int)i) {
					selected_texture = (int)i;
					new_texture_path = "images/" + available_textures[i];
					texture_changed = true;
				}
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Text("Available Textures: %zu", available_textures.size());
	ImGui::Checkbox("Flip Texture (V)", &flip_texture);
	ImGui::End();

	ImGui::Render();
}

void imgui_draw()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool imgui_model_changed()
{
	bool changed = model_changed;
	model_changed = false;
	return changed;
}

std::string imgui_get_selected_model()
{
	return current_model_path;
}

bool imgui_texture_changed()
{
	bool changed = texture_changed;
	texture_changed = false;
	return changed;
}

std::string imgui_get_texture_path()
{
	return new_texture_path;
}

void imgui_reset_texture_selection()
{
	selected_texture = -1;
}

bool imgui_get_flip_texture()
{
	return flip_texture;
}

void imgui_shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
