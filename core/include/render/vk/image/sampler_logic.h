#pragma once

#include <vulkan/vulkan.h>

class Context;

namespace Render {

	class SamplerLogic final {
	public:

		static VkSampler Create(Context*,
			float maxLod);

		static void Destroy(Context*, VkSampler&);

	};

}