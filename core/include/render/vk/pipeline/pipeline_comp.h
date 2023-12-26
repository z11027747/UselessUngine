#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include "render/vk/buffer/buffer_comp.h"

namespace Render {

	struct Uniform;

	struct GraphicsPipeline final {
		std::string name;

		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;
		VkDescriptorPool descriptorPool;

		std::vector<VkShaderModule> shaderModules;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfo;

		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<std::shared_ptr<Uniform>> uniforms;
	};

	struct Uniform {
		virtual ~Uniform() {}  
		VkDescriptorSet descriptorSet;
	};

	struct UniformTest final : public Uniform {
		std::shared_ptr<Buffer> uniform0Buffer;
		std::shared_ptr<Buffer> uniform1Buffer;
		std::shared_ptr<Image2D> image2D;
		VkSampler textureSampler;
	};

	struct UniformCameraTransform final {
		glm::mat4 view;
		glm::mat4 proj;
	};

	struct UniformObjectTransform final {
		glm::mat4 model;
	};


}