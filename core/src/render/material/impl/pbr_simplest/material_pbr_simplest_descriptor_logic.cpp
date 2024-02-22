
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_pbr_simplest_logic.h"
#include "define.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	void MaterialPBRSimplestDescriptorLogic::CreateSetLayout(Context *context,
															 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkDescriptorSetLayoutBinding materialUBO = {
			0, // binding
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(materialUBO);

		graphicsPipeline->descriptorBindings = bindings;
		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
	}

	void MaterialPBRSimplestDescriptorLogic::AllocateAndUpdate(Context *context,
															   std::shared_ptr<MaterialInstance> instance)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &info = instance->info;
		auto &graphicsPipeline = global->pipelineMap[info->pipelineName];
		auto &descriptorSetLayout = graphicsPipeline->descriptorSetLayout;

		auto descriptor = std::make_shared<Descriptor>();

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);
		descriptor->set = descriptorSet;

		// buffer
		VkDescriptorBufferInfo bufferInfo = {
			instance->buffer->vkBuffer,
			0,
			instance->buffer->size};
		descriptor->bufferInfos.push_back(bufferInfo);

		instance->descriptor = descriptor;

		DescriptorSetLogic::Update(context,
								   [=](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   auto &bindings = graphicsPipeline->descriptorBindings;
									   DescriptorSetLogic::WriteBuffer(writes, descriptor->set, 0,
																	   bindings[0].descriptorType, descriptor->bufferInfos[0]);
								   });
	}
	void MaterialPBRSimplestDescriptorLogic::Destroy(Context *context,
													 std::shared_ptr<MaterialInstance> instance)
	{
	}
}