
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/post_process/post_process_comp.h"
#include "editor/wrap/component_wrap.h"
#include "context.hpp"

namespace Editor
{
	static bool enableToonMapping = false;
	static bool enableBloom = false;

	template <>
	void ComponentWrap<Render::PostProcess>::Draw(Context *context,
												  std::shared_ptr<void> component, bool isInit)
	{
		auto postProcess = std::static_pointer_cast<Render::PostProcess>(component);

		auto &toonMappingParams = postProcess->toonMappingParams;
		auto &bloomParams = postProcess->bloomParams;

		if (isInit)
		{
			enableToonMapping = (postProcess->toonMappingParams.w == 1.0f);
			enableBloom = (postProcess->bloomParams.w == 1.0f);
			return;
		}

		ImGui::Spacing();
		if (ImGui::Checkbox("Toon Mapping", &enableToonMapping))
		{
			toonMappingParams.w = enableToonMapping ? 1.0f : 0.0f;
		}
		if (enableToonMapping)
		{
			ImGui::SliderFloat("Brightness", &toonMappingParams.x, 0.0f, 3.0f);
			ImGui::SliderFloat("Saturation", &toonMappingParams.y, 0.0f, 3.0f);
			ImGui::SliderFloat("Contrast", &toonMappingParams.z, 0.0f, 3.0f);
		}

		ImGui::Spacing();
		if (ImGui::Checkbox("Bloom", &enableBloom))
		{
			bloomParams.w = enableBloom ? 1.0f : 0.0f;
		}
		if (enableBloom)
		{
			ImGui::SliderFloat("Scale", &bloomParams.x, 0.0f, 30.0f);
			ImGui::SliderFloat("Threshold", &bloomParams.y, 0.0f, 1.0f);
			ImGui::InputFloat("Intensity", &bloomParams.z);
		}
	}
}