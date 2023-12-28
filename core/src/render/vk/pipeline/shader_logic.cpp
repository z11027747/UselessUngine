
#include <array>
#include "common/log_system.h"
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/vk/pipeline/shader_test_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/unit/unit_comp.h"
#include "logic/camera/camera_comp.h"
#include "common/res_system.h"
#include "context.h"
#include "engine_object.h"

namespace Render {

	void ShaderLogic::CreateVertexAttrDescriptions(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& name = graphicsPipeline->name;
		if (name == "test") {
			ShaderTestLogic::CreateVertexAttrDescriptions(context,
				graphicsPipeline);
		}
	}

	void ShaderLogic::CreateModules(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto& name = graphicsPipeline->name;
		auto& shaderModules = graphicsPipeline->shaderModules;
		auto& shaderStageCreateInfo = graphicsPipeline->shaderStageCreateInfo;

		std::array<VkShaderStageFlagBits, 2> stages = {
			VK_SHADER_STAGE_VERTEX_BIT,
			VK_SHADER_STAGE_FRAGMENT_BIT
		};

		std::array<std::vector<char>, 2> codes = {
			Common::ResSystem::ReadFile("resource/spv/" + name + ".vert.spv"),
			Common::ResSystem::ReadFile("resource/spv/" + name + ".frag.spv")
		};

		shaderModules.resize(2);
		shaderStageCreateInfo.resize(2);

		for (auto i = 0; i < 2; i++) {
			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = codes[i].size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(codes[i].data());

			VkShaderModule shaderModule;
			auto vertRet = vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule);
			CheckRet(vertRet, "vkCreateShaderModule-Vert");

			VkPipelineShaderStageCreateInfo stageCreateInfo = {};
			stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageCreateInfo.stage = stages[i];
			stageCreateInfo.module = shaderModule;
			stageCreateInfo.pName = "main";

			shaderModules[i] = shaderModule;
			shaderStageCreateInfo[i] = stageCreateInfo;
		}
	}

	void ShaderLogic::DestroyModules(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto& shaderModules = graphicsPipeline->shaderModules;
		for (auto& shaderModule : shaderModules) {
			vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
		}
	}

	void ShaderLogic::CreateDescriptorSetLayout(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		VkDescriptorSetLayoutBinding globalUniformBinding0 = {};
		globalUniformBinding0.binding = 0;
		globalUniformBinding0.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		globalUniformBinding0.descriptorCount = 1;
		globalUniformBinding0.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		std::vector<VkDescriptorSetLayoutBinding> globalBindings;
		globalBindings.push_back(globalUniformBinding0);

		graphicsPipeline->globalDescriptorSetLayout
			= DescriptorSetLayoutLogic::Create(context, globalBindings);

		auto& name = graphicsPipeline->name;
		if (name == "test") {
			ShaderTestLogic::CreateDescriptorSetLayout(context,
				graphicsPipeline);
		}
	}

	void ShaderLogic::DestroyDescriptorSetLayout(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& globalDescriptorSetLayout = graphicsPipeline->globalDescriptorSetLayout;
		DescriptorSetLayoutLogic::Destroy(context, globalDescriptorSetLayout);

		auto& name = graphicsPipeline->name;
		if (name == "test") {
			ShaderTestLogic::DestroyDescriptorSetLayout(context,
				graphicsPipeline);
		}
	}

	void ShaderLogic::CreateUniforms(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& name = graphicsPipeline->name;
		if (name == "test") {
			ShaderTestLogic::CreateUniforms(context,
				graphicsPipeline);
		}

		auto& globalDescriptorSetLayout = graphicsPipeline->globalDescriptorSetLayout;
		auto globalDescriptorSets = DescriptorSetLogic::Allocate(context, globalDescriptorSetLayout);

		auto& uniforms = graphicsPipeline->uniforms;
		for (auto i = 0; i < uniforms.size(); i++) {
			auto& uniform = uniforms[i];

			auto globalDescriptor = std::make_shared<Descriptor>();

			auto buffer = BufferLogic::Create(context,
				sizeof(GlobalUBO), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = buffer->vkBuffer;
			bufferInfo.offset = 0;
			bufferInfo.range = buffer->size;

			globalDescriptor->set = globalDescriptorSets[i];
			globalDescriptor->buffer = buffer;
			globalDescriptor->bufferInfo = bufferInfo;

			uniform->globalDescriptor = globalDescriptor;
		}
	}

	void ShaderLogic::DestroyUniforms(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto maxFrameInFlight = global->maxFrameInFlight;

		auto& uniforms = graphicsPipeline->uniforms;
		for (auto i = 0u; i < maxFrameInFlight; i++) {
			auto& uniform = uniforms[i];

			BufferLogic::Destroy(context,
				uniform->globalDescriptor->buffer);
		}

		auto& name = graphicsPipeline->name;
		if (name == "test") {
			ShaderTestLogic::DestroyUniforms(context,
				graphicsPipeline);
		}
	}

	void ShaderLogic::UpdateDescriptorSets(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;

		for (auto i = 0u; i < maxFrameInFlight; i++) {
			auto& uniform = graphicsPipeline->uniforms[i];

			DescriptorSetLogic::Update(context,
				[&](std::vector<VkWriteDescriptorSet>& writes) {

					DescriptorSetLogic::WriteBuffer(context,
						writes,
						uniform->globalDescriptor );

				});
		}

		auto& name = graphicsPipeline->name;
		if (name == "test") {
			ShaderTestLogic::UpdateDescriptorSets(context,
				graphicsPipeline);
		}
	}

	void ShaderLogic::UpdateUniformBuffer(Context* context,
		std::shared_ptr<EngineObject> unitEO,
		std::shared_ptr<Uniform> uniform
	) {
		auto cameraEO = context->GetEO(G_Camera);
		auto camera = cameraEO->GetComponent<Logic::Camera>();

		GlobalUBO globalUBO = {
			camera->view,
			camera->projection
		};

		BufferLogic::Set(context,
			uniform->globalUniform0Buffer,
			globalUBO);

		auto unit = unitEO->GetComponent<Render::Unit>();
		auto& name = unit->pipelineName;
		if (name == "test") {
			ShaderTestLogic::UpdateUniformBuffer(context,
				unitEO,
				uniform);
		}
	}

}