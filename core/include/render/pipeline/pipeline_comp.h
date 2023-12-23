#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace Render {

	struct Shader;
	struct Vertex;

	struct Pipeline final {

		VkPipeline vkPipeline;
		VkPipelineLayout pipelineLayout;
		std::shared_ptr<Shader> shader;
		VkDescriptorSetLayout descriptorSetLayout;

		//std::vector<VkDescriptorSet> descriptorSets;
	};

	struct Shader final {
		std::vector<VkShaderModule> modules;
		std::vector<VkPipelineShaderStageCreateInfo> stageCreateInfo;
	};

	struct Vertex final {
		glm::vec3 positionOS;
		glm::vec3 color;
		glm::vec2 uv0;
	};

}