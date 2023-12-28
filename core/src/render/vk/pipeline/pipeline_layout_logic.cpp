
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/vk/pipeline/pipeline_layout_logic.h"
#include "common/res_system.h"
#include "context.h"
#include "engine_object.h"

namespace Render {

	void PipelineLayoutLogic::Create(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		VkPipelineLayoutCreateInfo layoutCreateInfo = {};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts = {
			graphicsPipeline->globalDescriptorSetLayout,
			graphicsPipeline->descriptorSetLayout
		};
		layoutCreateInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		layoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();

		VkPushConstantRange pushConstantRange = {};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(glm::mat4);

		layoutCreateInfo.pushConstantRangeCount = 1;
		layoutCreateInfo.pPushConstantRanges = &pushConstantRange;

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