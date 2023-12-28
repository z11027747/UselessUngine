#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"

namespace Render {

	struct Descriptor final {
		VkDescriptorSet set;

		std::shared_ptr<Buffer> buffer;
		VkDescriptorBufferInfo bufferInfo;

		std::shared_ptr<Image> image;
		VkDescriptorImageInfo imageInfo;
	};

	struct GraphicsPipeline final {
		std::string name;

		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;

		std::vector<VkShaderModule> shaderModules;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfo;
		std::vector<VkVertexInputAttributeDescription> vertexAttrDescriptions;

		VkDescriptorSetLayout globalDescriptorSetLayout;
		std::vector<std::shared_ptr<Descriptor>> globalDescriptors;

		VkDescriptorSetLayout descriptorSetLayout;
	};

	struct GlobalUBO final {
		glm::mat4 view;
		glm::mat4 proj;
	};


}