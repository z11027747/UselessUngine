#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "render/vk/pipeline/pipeline_comp.h"

class Context;

namespace Render {

	class DescriptorPoolLogic final {
	public:

		static void Create(Context*,
			std::shared_ptr<GraphicsPipeline>
		);
		static void Destroy(Context*,
			std::shared_ptr<GraphicsPipeline>
		);

	};

}