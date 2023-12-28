#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/image/image_comp.h"

namespace Render {

	struct Uniform;

	struct GraphicsPipeline final {
		std::string name;

		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;

		std::vector<VkShaderModule> shaderModules;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfo;
		std::vector<VkVertexInputAttributeDescription> vertexAttrDescriptions;

		VkDescriptorSetLayout globalDescriptorSetLayout;
		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<std::shared_ptr<Uniform>> uniforms;
	};

	struct Descriptor final {
		VkDescriptorSet set;
		std::shared_ptr<Buffer> buffer;
		VkDescriptorBufferInfo bufferInfo;
	};

	struct Uniform {
		virtual ~Uniform() {}
		std::shared_ptr<Descriptor> globalDescriptor;
		std::shared_ptr<Descriptor> descriptor;
	};

	struct UniformTest final : public Uniform {
		VkSampler sampler;
	};

	struct UniformSkybox final : public Uniform {
		VkSampler samplerCube;
	};

	struct GlobalUBO final {
		glm::mat4 view;
		glm::mat4 proj;
	};


}