#pragma once

#include <memory>
#include "render/vk/pipeline/pipeline_comp.h"

class Context;
class EngineObject;

namespace Render {

	class ShaderLogic final {
	public:

		static void CreateVertexAttrDescriptions(Context*,
			std::shared_ptr<GraphicsPipeline>);

		static void CreateModules(Context*,
			std::shared_ptr<GraphicsPipeline>);
		static void DestroyModules(Context*,
			std::shared_ptr<GraphicsPipeline>);

		static void CreateDescriptorSetLayout(Context*,
			std::shared_ptr<GraphicsPipeline>);
		static void DestroyDescriptorSetLayout(Context*,
			std::shared_ptr<GraphicsPipeline>);

		static void CreateUniforms(Context*,
			std::shared_ptr<GraphicsPipeline>);
		static void DestroyUniforms(Context*,
			std::shared_ptr<GraphicsPipeline>);

		static void UpdateDescriptorSets(Context*,
			std::shared_ptr<GraphicsPipeline>);

		static void UpdateUniformBuffer(Context*,
			std::shared_ptr<EngineObject>,
			std::shared_ptr<Uniform>);

	};

}