#pragma once

#include <vulkan/vulkan.h>
#include <memory>

class Context;

namespace Render {

	class SwapchainSystem final {
	public:

		static void Create(Context*);
		static void Destroy(Context*);

		static void CreateColorImageViews(Context*);
		static void DestroyColorImageViews(Context*);

		static void CreateDepthImageViews(Context*);
		static void DestroyDepthImageViews(Context*);

	};


}