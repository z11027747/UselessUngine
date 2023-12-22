#pragma once

#include <string>
#include <array>
#include "render/pipeline/pipeline_comp.h"

class Context;

namespace Render {

	class PipelineSystem final {
	public:

		static void Create(Context*,
			const std::string&
		);

		static void DestroyAll(Context*);

		static void GreateGraphicsPipeline(Context* context,
			std::shared_ptr<Pipeline> pipeline
		);

		static void MakeVertexInputCreateInfo(
			VkVertexInputBindingDescription&,
			std::vector<VkVertexInputAttributeDescription>&,
			VkPipelineVertexInputStateCreateInfo&
		);
		static void MakeInputAssemblyCreateInfo(
			VkPipelineInputAssemblyStateCreateInfo&
		);
		static void MakeViewportCreateInfo(
			VkExtent2D&,
			VkViewport&, VkRect2D&,
			VkPipelineViewportStateCreateInfo&
		);
		static void MakeRasterizationCreateInfo(
			VkPipelineRasterizationStateCreateInfo&
		);
		static void MakeDepthStencilCreateInfo(
			VkPipelineDepthStencilStateCreateInfo&
		);
		static void MakeMultisampleCreateInfo(
			VkPipelineMultisampleStateCreateInfo&
		);
		static void MakeColorBlendCreateInfo(
			VkPipelineColorBlendAttachmentState&,
			VkPipelineColorBlendStateCreateInfo&
		);


	};

}