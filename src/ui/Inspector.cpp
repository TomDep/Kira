#include "Inspector.h"

#include <imgui.h>
#include <string>
#include <glm/glm.hpp>

#include "Utils.hpp"

Inspector::Inspector(Odometry* odometry) {
	this->odometry = odometry;
}

void Inspector::render() {
	//ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3.0f, 3.0f));
	ImGui::Begin("Inspector", nullptr, windowFlags);
	ImGui::PopStyleVar();
	
	if (ImGui::CollapsingHeader("Odometry", ImGuiTreeNodeFlags_DefaultOpen)) {	
		glm::vec2 position = odometry->getPosition();
		float angle = rad2deg(odometry->getAngle());

		ImGui::Text("Position :");
		ImGui::Text("X: %.0fmm\tY: %.0fmm", position.x, position.y);
		ImGui::Text("Angle : %.2f°", angle);
	}
	
	ImGui::End();
}
