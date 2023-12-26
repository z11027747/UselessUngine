
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/vk/pipeline/pipeline_layout_logic.h"
#include "common/res_system.h"
#include "context.h"
#include "base.h"

namespace Render {

	void PipelineLayoutLogic::Create(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		VkPipelineLayoutCreateInfo layoutCreateInfo = {};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		auto& descriptorSetLayout = graphicsPipeline->descriptorSetLayout;
		layoutCreateInfo.setLayoutCount = 1;
		layoutCreateInfo.pSetLayouts = &descriptorSetLayout;

		VkPipelineLayout pipelineLayout;
		auto ret = vkCreatePipelineLayout(logicalDevice, &layoutCreateInfo, nullptr, &pipelineLayout);
		CheckRet(ret, "vkCreatePipelineLayout");

		graphicsPipeline->pipelineLayout = pipelineLayout;
	}

	void PipelineLayoutLogic::Destroy(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& pipelineLayout = graphicsPipeline->pipelineLayout;
		vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
	}

}