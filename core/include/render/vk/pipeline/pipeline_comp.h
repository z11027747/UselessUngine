#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <memory>
#include "render/vk/pipeline/descriptor_comp.h"

namespace Render
{
	struct PipelineStageInfo final
	{
		std::vector<VkShaderModule> shaderModules;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos;
		std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions;
		std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions;
		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
		VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo;
		std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachmentStates;
		VkPipelineColorBlendStateCreateInfo colorBlendingStateCreateInfo;
	};

	struct GraphicsPipeline final
	{
		std::string name;
		PipelineStageInfo stageInfo;
		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkDescriptorSetLayoutBinding> descriptorBindings;
		VkDescriptorSetLayout descriptorSetLayout;
		uint32_t subpass{0};

		std::shared_ptr<GraphicsPipeline> instance;
	};
}