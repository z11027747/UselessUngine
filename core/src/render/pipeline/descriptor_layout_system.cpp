
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/device/logical_device_system.h"
#include "render/global/device/physical_device_system.h"
#include "render/global/swapchain/swapchain_system.h"
#include "render/pipeline/pipeline_comp.h"
#include "render/pipeline/pipeline_system.h"
#include "render/pipeline/descriptor_layout_system.h"
#include "common/res_system.h"
#include "context.h"
#include "base.h"

namespace Render {

	void DescriptorLayoutSystem::Create(Context* context,
		std::shared_ptr<Pipeline> pipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = {};

		VkDescriptorSetLayoutBinding uniformBinding0 = {};
		uniformBinding0.binding = 0;
		uniformBinding0.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uniformBinding0.descriptorCount = 1;
		uniformBinding0.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		bindings[0] = uniformBinding0;

		VkDescriptorSetLayoutBinding samplerBinding1 = {};
		samplerBinding1.binding = 1;
		samplerBinding1.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerBinding1.descriptorCount = 1;
		samplerBinding1.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		bindings[1] = samplerBinding1;

		VkDescriptorSetLayoutCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		createInfo.pBindings = bindings.data();

		VkDescriptorSetLayout descriptorSetLayout;
		auto ret = vkCreateDescriptorSetLayout(logicalDevice, &createInfo, nullptr, &descriptorSetLayout);
		CheckRet(ret, "vkCreateDescriptorSetLayout");

		pipeline->descriptorSetLayout = descriptorSetLayout;
	}

	void DescriptorLayoutSystem::Destroy(Context* context,
		std::shared_ptr<Pipeline> pipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		vkDestroyDescriptorSetLayout(logicalDevice, pipeline->descriptorSetLayout, nullptr);
	}

}