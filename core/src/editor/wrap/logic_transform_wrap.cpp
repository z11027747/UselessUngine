
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/wrap/component_wrap.h"
#include "editor/system.h"
#include "editor/test_logic.h"
#include "context.h"

namespace Editor
{
	static int pId = 0;
	static int eId = 1;
	static int sId = 2;

	void ComponentWrap<Logic::Transform>::Draw(Context *context,
											   std::shared_ptr<Logic::Transform> transform, bool isFirst)
	{
		ImGui_Drag_GlmVec3(transform->localPosition, pId, 0.02f);
		ImGui::SameLine();
		if (ImGui::Button("P", ImVec2(20, 20)))
		{
			transform->localPosition = glm::vec3(0.0f);
		}
		ImGui::SameLine();
		ImGui::Text("Position");

		ImGui_Drag_GlmVec3(transform->localEulerAngles, eId, 0.05f);
		ImGui::SameLine();
		if (ImGui::Button("E", ImVec2(20, 20)))
		{
			transform->localEulerAngles = glm::vec3(0.0f);
		}
		ImGui::SameLine();
		ImGui::Text("EulerAngles");

		ImGui_Drag_GlmVec3(transform->localScale, sId, 0.02f);
		ImGui::SameLine();
		if (ImGui::Button("S", ImVec2(20, 20)))
		{
			transform->localScale = glm::vec3(1.0f);
		}
		ImGui::SameLine();
		ImGui::Text("Scale");
	}
}