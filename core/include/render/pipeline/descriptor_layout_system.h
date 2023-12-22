#pragma once

#include <string>
#include <array>
#include <memory>
#include "render/pipeline/pipeline_comp.h"

class Context;

namespace Render {

	class DescriptorLayoutSystem final {
	public:

		static void Create(Context*,
			std::shared_ptr<Pipeline> pipeline
		);

		static void Destroy(Context*,
			std::shared_ptr<Pipeline> pipeline
		);

	};

}