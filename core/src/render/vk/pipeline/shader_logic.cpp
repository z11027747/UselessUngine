
#include <array>
#include "common/log_system.h"
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "common/res_system.h"
#include "context.h"
#include "base.h"

namespace Render {

	void ShaderLogic::Create(Context* context,
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

	void ShaderLogic::Destroy(Context* context,
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

	void ShaderLogic::CreateUniforms(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;

		auto& name = graphicsPipeline->name;
		auto& descriptorPool = graphicsPipeline->descriptorPool;
		auto& descriptorSetLayout = graphicsPipeline->descriptorSetLayout;

		VkDescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = descriptorPool;
		allocateInfo.descriptorSetCount = maxFrameInFlight;

		std::vector<VkDescriptorSetLayout> layouts(maxFrameInFlight, descriptorSetLayout);
		allocateInfo.pSetLayouts = layouts.data();

		std::vector<VkDescriptorSet> vkDescriptorSets(maxFrameInFlight);
		auto ret = vkAllocateDescriptorSets(logicalDevice, &allocateInfo, vkDescriptorSets.data());
		CheckRet(ret, "vkAllocateDescriptorSets");

		auto& uniforms = graphicsPipeline->uniforms;
		uniforms.resize(maxFrameInFlight);

		for (auto i = 0u; i < maxFrameInFlight; i++) {
			if (name != "test") {
				Common::LogSystem::Exception("error uniform: " + name);
				continue;
			}

			auto uniform_test = std::make_shared<UniformTest>();

			uniform_test->descriptorSet = vkDescriptorSets[i];

			auto uniform0Size = (sizeof(UniformCameraTransform));
			auto uniform1Size = (sizeof(UniformObjectTransform));

			uniform_test->uniform0Buffer = BufferLogic::Create(context,
				uniform0Size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			uniform_test->uniform1Buffer = BufferLogic::Create(context,
				uniform1Size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			std::array<VkWriteDescriptorSet, 2> writes = {};

			writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[0].dstSet = vkDescriptorSets[i];
			writes[0].dstBinding = 0;
			writes[0].dstArrayElement = 0;
			writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writes[0].descriptorCount = 1;

			VkDescriptorBufferInfo bufferInfo0 = {};
			bufferInfo0.buffer = uniform_test->uniform0Buffer->vkBuffer;
			bufferInfo0.offset = 0;
			bufferInfo0.range = uniform0Size;
			writes[0].pBufferInfo = &bufferInfo0;

			writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[1].dstSet = vkDescriptorSets[i];
			writes[1].dstBinding = 1;
			writes[1].dstArrayElement = 0;
			writes[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writes[1].descriptorCount = 1;

			VkDescriptorBufferInfo bufferInfo1 = {};
			bufferInfo1.buffer = uniform_test->uniform1Buffer->vkBuffer;
			bufferInfo1.offset = 0;
			bufferInfo1.range = uniform1Size;
			writes[1].pBufferInfo = &bufferInfo1;

			//writes[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			//writes[2].dstSet = vkDescriptorSets[i];
			//writes[2].dstBinding = 2;
			//writes[2].dstArrayElement = 0;
			//writes[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			//writes[2].descriptorCount = 1;

			//VkDescriptorImageInfo imageInfo = {};
			//imageInfo.sampler = textureSampler;
			//imageInfo.imageView = textureImageView;
			//imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			//writes[2].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);

			uniforms[i] = uniform_test;
		}
	}

	void ShaderLogic::DestroyUniforms(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;

		auto& name = graphicsPipeline->name;
		auto& descriptorPool = graphicsPipeline->descriptorPool;
		auto& descriptorSetLayout = graphicsPipeline->descriptorSetLayout;

		auto& uniforms = graphicsPipeline->uniforms;
		for (auto& uniform : uniforms) {

			auto uniform_test = std::dynamic_pointer_cast<UniformTest>(uniform);
			if (uniform_test) {

				BufferLogic::Destroy(context,
					uniform_test->uniform0Buffer);

				BufferLogic::Destroy(context,
					uniform_test->uniform1Buffer);

			}
		}
		uniforms.clear();
	}

}