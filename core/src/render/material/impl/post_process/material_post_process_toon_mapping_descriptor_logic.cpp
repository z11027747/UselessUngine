
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/material/impl/material_post_process_logic.h"
#include "context.hpp"

namespace Render
{
	void MaterialPostProcessToonMappingDescriptorLogic::CreateSetLayout(Context *context,
																		std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		std::vector<VkDescriptorSetLayoutBinding> bindings;

		VkDescriptorSetLayoutBinding blitImage = {
			0, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		bindings.push_back(blitImage);

		graphicsPipeline->descriptorBindings = bindings;
		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
		graphicsPipeline->subpass = 1;
	}

	void MaterialPostProcessToonMappingDescriptorLogic::AllocateAndUpdate(Context *context,
																		  std::shared_ptr<MaterialData> data)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &postProcessPass = global->passMap[Define::Pass::PostProcess];
		auto &postProcessPipeline = global->pipelineMap[Define::Pipeline::PostProcess_ToonMapping];

		auto descriptor = std::make_shared<Descriptor>();

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, postProcessPipeline->descriptorSetLayout);
		descriptor->set = descriptorSet;

		VkDescriptorImageInfo blitImageInfo = {
			global->globalSamplerClampLinear,
			postProcessPass->colorImage2ds[1]->vkImageView,
			postProcessPass->colorImage2ds[1]->layout};
		descriptor->imageInfos.push_back(blitImageInfo);

		DescriptorSetLogic::Update(context,
								   [=](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   auto &bindings = postProcessPipeline->descriptorBindings;
									   DescriptorSetLogic::WriteImage(writes, descriptor->set, 0,
																	  bindings[0].descriptorType, descriptor->imageInfos[0]);
								   });

		data->descriptor = descriptor;
	}
	void MaterialPostProcessToonMappingDescriptorLogic::Destroy(Context *context,
																std::shared_ptr<MaterialData> data)
	{
	}
}