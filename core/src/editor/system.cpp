
#include "editor/imgui/imgui_logic.h"
#include "editor/axis/axis_system.h"
#include "editor/wrap/wrap_system.hpp"
#include "editor/system.h"
#include "editor/window.h"
#include "context.hpp"

namespace Editor
{
	void System::Create(Context *context)
	{
		AxisCreateSystem::Create(context);
		WrapRegistSystem::Create(context);

		ImGuiLogic::CreateImGui(context);
		ImGuiLogic::CreateDescriptorSetLayout(context);
		ImGuiLogic::CreateDescriptors(context);
	}

	void System::Update(Context *context)
	{
		CameraMoveSystem::Update(context);
		CameraRotateSystem::Update(context);

		auto result = AxisSelectSystem::Update(context);
		if (!result)
		{
			EOSelectSystem::Update(context);
		}
	}

	void System::Destroy(Context *context)
	{
		ImGuiLogic::DestroyDescriptorSetLayout(context);
		ImGuiLogic::DestroyImGui(context);
	}
}