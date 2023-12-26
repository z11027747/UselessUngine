
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
		std::shared_ptr<Pipeline> pipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		VkPipelineLayoutCreateInfo layoutCreateInfo = {};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutCreateInfo.setLayoutCount = 0;

		//auto& descriptorSetLayout = pipeline->descriptorSetLayout;
		//layoutCreateInfo.setLayoutCount = 1;
		//layoutCreateInfo.pSetLayouts = &descriptorSetLayout;

		VkPipelineLayout pipelineLayout;
		auto ret = vkCreatePipelineLayout(logicalDevice, &layoutCreateInfo, nullptr, &pipelineLayout);
		CheckRet(ret, "vkCreatePipelineLayout");

		pipeline->pipelineLayout = pipelineLayout;
	}

	void PipelineLayoutLogic::Destroy(Context* context,
		std::shared_ptr<Pipeline> pipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto& pipelineLayout = pipeline->pipelineLayout;
		vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
	}

}