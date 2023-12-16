﻿#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "base.h"

struct RenderShader;

struct RenderGlobalComp final : public EngineComp {

	//VkInstance 底层就是个指针
	//#define VK_DEFINE_HANDLE(object)
	//	typedef struct object##_T* object;

	VkInstance instance;

	bool enabledDebug;
	VkDebugUtilsMessengerEXT debugCallback;

	VkPhysicalDevice physicalDevice;
	int physicalDeviceGraphicsFamily;

	VkDevice logicDevice;
	VkQueue logicQueue;

	VkSurfaceKHR surface;

	VkSwapchainKHR swapchain;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;

	std::unordered_map<std::string, std::shared_ptr<RenderShader>> shaderMap;

	VkPipeline graphicsPipeline;
	VkPipelineLayout graphicsPipelineLayout;
};

struct RenderShader final {

	std::vector<VkShaderModule> modules;
	std::vector<VkPipelineShaderStageCreateInfo> stageCreateInfos;

};