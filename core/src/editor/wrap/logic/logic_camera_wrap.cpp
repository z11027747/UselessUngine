
#include <imgui/imgui.h>
#include "render/vk/global/global_comp.h"
#include "logic/camera/camera_comp.h"
#include "editor/wrap/component_wrap.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Editor
{
	static int mode = 0;

	static int passNameIndex = -1;
	static const int passNameSize = 2;
	static const char *passNames[] =
		{
			Define::Pass::Forward.c_str(),
			Define::Pass::Deferred.c_str()};

	template <>
	void ComponentWrap<Logic::Camera>::Draw(Context *context,
											std::shared_ptr<void> component, bool isInit)
	{
		auto camera = std::static_pointer_cast<Logic::Camera>(component);

		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &pass = global->passMap[camera->passName];

		if (isInit)
		{
			auto &passName = pass->name;
			for (auto i = 0; i < passNameSize; i++)
			{
				if (strcmp(passNames[i], passName.c_str()) == 0)
				{
					passNameIndex = i;
					break;
				}
			}

			mode = static_cast<int>(camera->mode);
			return;
		}

		if (ImGui::DragFloat("Near", &camera->near, 0.01f, 0.01f, 10.0f))
		{
			camera->far = std::max(camera->far, camera->near + 0.01f);
		}

		ImGui::DragFloat("Far", &camera->far, 0.02f, camera->near + 0.01f, 100.0f);

		if (ImGui::Combo("Mode", &mode, "Perspective\0Ortho\0"))
		{
			camera->mode = static_cast<Logic::CameraMode>(mode);
		}

		if (camera->mode == Logic::CameraMode::ePerspective)
		{
			ImGui::DragFloat("fov", &camera->fov, 0.02f, 10.0f, 90.0f);
		}
		else if (camera->mode == Logic::CameraMode::eOrtho)
		{
			ImGui::DragFloat("Size", &camera->size, 0.02f);
		}

		ImGui::Spacing();
		ImGui::Text("Render Pass");

		if (ImGui::Combo("##passNames",
						 &passNameIndex, passNames, IM_ARRAYSIZE(passNames)))
		{
			camera->passName = passNames[passNameIndex];
		}

		ImGui::Spacing();
		ImGui::Text("Clear Values");

		auto clearValueSize = pass->clearValues.size();
		for (auto i = 0; i < clearValueSize; i++)
		{
			ImGui::Text("Attachment: %d", i);

			auto &attachmentDescription = pass->attachmentDescriptions[i];
			if (attachmentDescription.format == global->depthFormat ||
				attachmentDescription.format == global->depthStencilFormat)
			{
				auto &clearValue = pass->clearValues[i];
				ImGui::SliderFloat2("##Depth", &clearValue.depthStencil.depth, 0.0f, 0.1f);
			}
			else
			{
				auto &clearValue = pass->clearValues[i];
				ImGui::ColorEdit4("##Color", &clearValue.color.float32[0]);
			}
		}
	}
}