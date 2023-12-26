#pragma once

#include <memory>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/buffer/buffer_logic.h"

class Context;

namespace Render {

	class ShaderLogic final {
	public:

		static void Create(Context*,
			std::shared_ptr<GraphicsPipeline>
		);
		static void Destroy(Context*,
			std::shared_ptr<GraphicsPipeline>
		);

		static void CreateUniforms(Context*,
			std::shared_ptr<GraphicsPipeline>
		);
		static void DestroyUniforms(Context*,
			std::shared_ptr<GraphicsPipeline>
		);

		template<typename T, typename TValue>
		static void UpdateUniform0(Context* context,
			std::shared_ptr<GraphicsPipeline> graphicsPipeline,
			uint32_t index, TValue& value
		) {
			auto& renderGlobalEO = context->renderGlobalEO;

			auto global = renderGlobalEO->GetComponent<Global>();
			auto& logicalDevice = global->logicalDevice;

			auto& uniform = graphicsPipeline->uniforms[index];
			auto& uniform_spec = std::dynamic_pointer_cast<T>(uniform);

			BufferLogic::SetValue(context,
				uniform_spec->uniform0Buffer,
				value);
		}

		template<typename T, typename TValue>
		static void UpdateUniform1(Context* context,
			std::shared_ptr<GraphicsPipeline> graphicsPipeline,
			uint32_t index, TValue& value
		) {
			auto& renderGlobalEO = context->renderGlobalEO;

			auto global = renderGlobalEO->GetComponent<Global>();
			auto& logicalDevice = global->logicalDevice;

			auto& uniform = graphicsPipeline->uniforms[index];
			auto& uniform_spec = std::dynamic_pointer_cast<T>(uniform);

			BufferLogic::SetValue(context,
				uniform_spec->uniform1Buffer,
				value);
		}
	};

}