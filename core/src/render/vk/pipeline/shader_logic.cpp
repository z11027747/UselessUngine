
#include <array>
#include "common/log_system.h"
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/vk/pipeline/shader_impl/shader_color_logic.h"
#include "render/vk/pipeline/shader_impl/shader_skybox_logic.h"
#include "render/vk/pipeline/shader_impl/shader_bling_phone_logic.h"
#include "render/vk/pipeline/shader_impl/shader_shadow_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/unit/unit_comp.h"
#include "logic/camera/camera_comp.h"
#include "common/res_system.h"
#include "context.h"
#include "engine_object.h"

namespace Render
{
	void ShaderLogic::CreateVertexAttrDescriptions(Context *context,
												   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &name = graphicsPipeline->name;
		if (name == Pipeline_Color)
		{
			ShaderColorLogic::CreateVertexAttrDescriptions(context,
														   graphicsPipeline);
		}
		else if (name == Pipeline_Skybox)
		{
			ShaderSkyboxLogic::CreateVertexAttrDescriptions(context,
															graphicsPipeline);
		}
		else if (name == Pipeline_Bling_Phone)
		{
			ShaderBlingPhoneLogic::CreateVertexAttrDescriptions(context,
																graphicsPipeline);
		}
		else if (name == Pipeline_Shadow)
		{
			ShaderShadowLogic::CreateVertexAttrDescriptions(context,
															graphicsPipeline);
		}
	}

	void ShaderLogic::SetViewport(Context *context,
								  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &name = graphicsPipeline->name;
		if (name == Pipeline_Shadow)
		{
			ShaderShadowLogic::SetViewport(context,
										   graphicsPipeline);
		}
	}

	void ShaderLogic::SetRasterizationCreateInfo(Context *context,
												 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &name = graphicsPipeline->name;
		if (name == Pipeline_Color)
		{
			ShaderColorLogic::SetRasterizationCreateInfo(context,
														 graphicsPipeline);
		}
		else if (name == Pipeline_Skybox)
		{
			ShaderSkyboxLogic::SetRasterizationCreateInfo(context,
														  graphicsPipeline);
		}
		else if (name == Pipeline_Bling_Phone)
		{
			ShaderBlingPhoneLogic::SetRasterizationCreateInfo(context,
															  graphicsPipeline);
		}
		else if (name == Pipeline_Shadow)
		{
			ShaderShadowLogic::SetRasterizationCreateInfo(context,
														  graphicsPipeline);
		}
	}

	void ShaderLogic::SetDepthStencilCreateInfo(Context *context,
												std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &name = graphicsPipeline->name;
		if (name == Pipeline_Skybox)
		{
			ShaderSkyboxLogic::SetDepthStencilCreateInfo(context,
														 graphicsPipeline);
		}
		else if (name == Pipeline_Color)
		{
			ShaderColorLogic::SetDepthStencilCreateInfo(context,
														graphicsPipeline);
		}
	}

	void ShaderLogic::CreateGlobalDescriptorSetLayout(Context *context,
													  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkDescriptorSetLayoutBinding globalUniformBinding0 = {};
		globalUniformBinding0.binding = 0;
		globalUniformBinding0.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		globalUniformBinding0.descriptorCount = 1;
		globalUniformBinding0.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

		std::vector<VkDescriptorSetLayoutBinding> globalBindings;
		globalBindings.push_back(globalUniformBinding0);

		graphicsPipeline->globalDescriptorSetLayout = DescriptorSetLayoutLogic::Create(context, globalBindings);
	}

	void ShaderLogic::DestroyGlobalDescriptorSetLayout(Context *context,
													   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalDescriptorSetLayout = graphicsPipeline->globalDescriptorSetLayout;
		DescriptorSetLayoutLogic::Destroy(context, globalDescriptorSetLayout);
	}

	void ShaderLogic::CreateDescriptorSetLayout(Context *context,
												std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &name = graphicsPipeline->name;
		if (name == Pipeline_Skybox)
		{
			ShaderSkyboxLogic::CreateDescriptorSetLayout(context,
														 graphicsPipeline);
		}
		else if (name == Pipeline_Bling_Phone)
		{
			ShaderBlingPhoneLogic::CreateDescriptorSetLayout(context,
															 graphicsPipeline);
		}
	}

	void ShaderLogic::DestroyDescriptorSetLayout(Context *context,
												 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &name = graphicsPipeline->name;
		if (name == Pipeline_Skybox)
		{
			ShaderSkyboxLogic::DestroyDescriptorSetLayout(context,
														  graphicsPipeline);
		}
		else if (name == Pipeline_Bling_Phone)
		{
			ShaderBlingPhoneLogic::DestroyDescriptorSetLayout(context,
															  graphicsPipeline);
		}
	}

	void ShaderLogic::CreateGlobalDescriptor(Context *context,
											 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto swapchainImageCount = global->swapchainImageCount;

		auto &globalDescriptorSetLayout = graphicsPipeline->globalDescriptorSetLayout;
		auto globalDescriptorSet = DescriptorSetLogic::AllocateOne(context, globalDescriptorSetLayout);

		auto globalUBOSize = sizeof(GlobalUBO);

		auto globalDescriptor = std::make_shared<Descriptor>();

		auto globalBuffer = BufferLogic::Create(context,
												globalUBOSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
												VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VkDescriptorBufferInfo bufferInfo = {
			globalBuffer->vkBuffer,
			0,
			globalUBOSize};

		globalDescriptor->set = globalDescriptorSet;
		globalDescriptor->bufferInfo = bufferInfo;

		graphicsPipeline->globalDescriptor = globalDescriptor;
		graphicsPipeline->globalBuffer = globalBuffer;
	}

	void ShaderLogic::DestroyGlobalDescriptor(Context *context,
											  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		BufferLogic::Destroy(context, graphicsPipeline->globalBuffer);
	}

	void ShaderLogic::UpdateGlobalDescriptor(Context *context,
											 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		DescriptorSetLogic::Update(context,
								   [&](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   DescriptorSetLogic::WriteBuffer(context,
																	   writes,
																	   graphicsPipeline->globalDescriptor);
								   });
	}

	void ShaderLogic::CreateUnitDescriptor(Context *context,
										   std::shared_ptr<Unit> unit)
	{
		auto &name = unit->pipelineName;
		if (name == Pipeline_Skybox)
		{
			ShaderSkyboxLogic::CreateUnitDescriptor(context,
													unit);
		}
		else if (name == Pipeline_Bling_Phone)
		{
			ShaderBlingPhoneLogic::CreateUnitDescriptor(context,
														unit);
		}
	}

	void ShaderLogic::DestroyUnitDescriptor(Context *context,
											std::shared_ptr<Unit> unit)
	{
		auto &name = unit->pipelineName;
		if (name == Pipeline_Skybox)
		{
			ShaderSkyboxLogic::DestroyUnitDescriptor(context,
													 unit);
		}
		else if (name == Pipeline_Bling_Phone)
		{
			ShaderBlingPhoneLogic::DestroyUnitDescriptor(context,
														 unit);
		}
	}

	void ShaderLogic::UpdateUnitDescriptor(Context *context,
										   std::shared_ptr<Unit> unit,
										   uint32_t imageIndex)
	{
		auto &name = unit->pipelineName;
		if (name == Pipeline_Bling_Phone)
		{
			ShaderBlingPhoneLogic::UpdateUnitDescriptor(context,
														unit,
														imageIndex);
		}
	}
}