#include "MainMenuBar.h"

#include <imgui.h>

void MainMenuBar::render() {
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Close", "Alt + F4");
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene"))
		{
			ImGui::MenuItem("Open a scene", "Ctr + O");
			ImGui::MenuItem("Save the scene", "Ctr + S");
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Options"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}