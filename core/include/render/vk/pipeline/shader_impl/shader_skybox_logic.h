#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "render/vk/pipeline/pipeline_comp.h"

class Context;
class EngineObject;

namespace Render
{
	class ShaderSkyboxLogic final
	{
	public:
		static void CreateVertexAttrDescriptions(Context *, std::shared_ptr<GraphicsPipeline>);
		static void SetRasterizationCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
		static void SetDepthStencilCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);

		static void CreateDescriptorSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
		static void DestroyDescriptorSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);

		static void CreateUnitDescriptor(Context *, std::shared_ptr<Unit>);
		static void DestroyUnitDescriptor(Context *, std::shared_ptr<Unit>);
	};
}