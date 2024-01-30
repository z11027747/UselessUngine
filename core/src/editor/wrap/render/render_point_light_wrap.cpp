
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/light/light_comp.h"
#include "editor/wrap/component_wrap.h"
#include "editor/imgui_logic.h"
#include "context.h"

namespace Editor
{
	template <>
	void ComponentWrap<Render::PointLight>::Draw(Context *context,
												 std::shared_ptr<Render::PointLight> pointLight, bool isInit)
	{
		if (isInit)
		{
			return;
		}

		if (ImGui::ColorEdit3("Color", &pointLight->color.x))
		{
		}

		ImGui::InputFloat("Constant", &pointLight->clq.x);
		ImGui::InputFloat("Linear", &pointLight->clq.y);
		ImGui::InputFloat("Quadratic", &pointLight->clq.z, 0.0f, 0.0f, "%.4f");

		ImGui::Checkbox("HasShadow", &pointLight->hasShadow);
	}
}