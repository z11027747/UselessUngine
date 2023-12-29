#pragma once

#include <memory>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/unit/unit_comp.h"

class Context;
class EngineObject;

namespace Render {

	class ShaderLogic final {
	public:

		static void CreateModules(Context*,
			std::shared_ptr<GraphicsPipeline>);
		static void DestroyModules(Context*,
			std::shared_ptr<GraphicsPipeline>);

		static void CreateVertexAttrDescriptions(Context*,
			std::shared_ptr<GraphicsPipeline>);
		static void MakeDepthStencilCreateInfo(Context*,
			std::shared_ptr<GraphicsPipeline>,
			VkPipelineDepthStencilStateCreateInfo&);
		static void MakeRasterizationCreateInfo(Context*,
			std::shared_ptr<GraphicsPipeline>,
			VkPipelineRasterizationStateCreateInfo&);

		static void CreateDescriptorSetLayout(Context*,
			std::shared_ptr<GraphicsPipeline>);
		static void DestroyDescriptorSetLayout(Context*,
			std::shared_ptr<GraphicsPipeline>);

		static void CreateDescriptors(Context*,
			std::shared_ptr<GraphicsPipeline>);
		static void DestroyDescriptors(Context*,
			std::shared_ptr<GraphicsPipeline>);

		static void UpdateDescriptorSets(Context*,
			std::shared_ptr<GraphicsPipeline>);

		static void UpdateDescriptor(Context*,
			std::shared_ptr<Descriptor>,
			std::shared_ptr<Unit>);

		static void CreateUnitDescriptor(Context*,
			std::shared_ptr<Unit>,
			std::shared_ptr<Image>);
		static void DestroyUnitDescriptor(Context*,
			std::shared_ptr<Unit>);

	};

}