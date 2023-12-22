#pragma once

#include <string>
#include <array>
#include <memory>
#include "render/pipeline/pipeline_comp.h"

class Context;

namespace Render {

	class ShaderSystem final {
	public:

		static void Create(Context*,
			const std::string&,
			std::shared_ptr<Pipeline>
		);

		static void Destroy(Context*,
			std::shared_ptr<Pipeline>
		);

	};

}