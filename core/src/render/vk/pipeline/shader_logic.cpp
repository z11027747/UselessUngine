
#include <array>
#include "common/log_system.h"
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/vk/pipeline/shader_test_logic.h"
#include "render/unit/unit_comp.h"
#include "common/res_system.h"
#include "context.h"
#include "base.h"

namespace Render {

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

	void ShaderLogic::CreateVertexAttrDescriptions(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& name = graphicsPipeline->name;
		if (name == "test") {
			ShaderTestLogic::CreateVertexAttrDescriptions(context,
				graphicsPipeline);
		}
	}

	void ShaderLogic::CreateDescriptorSetLayout(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& name = graphicsPipeline->name;
		if (name == "test") {
			ShaderTestLogic::CreateDescriptorSetLayout(context,
				graphicsPipeline);
		}
	}

	void ShaderLogic::DestroyDescriptorSetLayout(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
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
	}

	void ShaderLogic::DestroyUniforms(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& name = graphicsPipeline->name;
		if (name == "test") {
			ShaderTestLogic::DestroyUniforms(context,
				graphicsPipeline);
		}
	}

	void ShaderLogic::UpdateDescriptorSets(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
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
		auto unit = unitEO->GetComponent<Render::Unit>();
		auto& name = unit->pipelineName;
		if (name == "test") {
			ShaderTestLogic::UpdateUniformBuffer(context,
				unitEO,
				uniform);
		}
	}

}