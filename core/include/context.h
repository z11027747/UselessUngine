#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include "base.h"
#include "render/system_new.h"
#include "render/system.h"
#include "tool/system.h"

class Context final {
public:
	GLFWwindow* window;

	Context(GLFWwindow* w) {
		window = w;
		currTime = 0.0f;
		deltaTime = 0.0f;
	}

	//time
	float currTime;
	float deltaTime;

	//render
	std::shared_ptr<EngineObject> renderEO;

	//render_new
	std::shared_ptr<EngineObject> renderGlobalEO;
	std::shared_ptr<EngineObject> renderCmdSubmitEO;
	std::vector<std::shared_ptr<EngineObject>> renderCmdSubmitSemaphoreEOs;

	void Create() {
		//render
		RenderSystem::CreateGlobal(this);
		RenderSystem::CreateInstance(this);
		//RenderSystem::CreateDebugCallback(this);
		RenderSystem::CreateSurface(this);
		RenderSystem::PickupPhysicalDevice(this);
		RenderSystem::CreateLogicDevice(this);
		RenderSystem::GetLogicDeviceQueue(this);
		RenderSystem::CreateSwapchian(this);
		RenderSystem::GetSwapchianImages(this);
		RenderSystem::CreateSwapchianImageViews(this);
		RenderSystem::CreateShader(this, "test");
		RenderSystem::CreateDescriptorSetLayout(this);
		RenderSystem::CreatePipelineLayout(this);
		RenderSystem::CreateCommandPool(this);
		RenderSystem::CreateDepth(this);
		RenderSystem::CreateRenderPass(this);
		RenderSystem::CreateGraphicsPipeline(this);
		RenderSystem::CreateFrameBuffers(this);
		RenderSystem::AllocateSwapchainCommandBuffers(this);
		RenderSystem::CreateSemaphores(this);
		RenderSystem::CreateFences(this);
		//RenderSystem::CheckPhysicalDeviceMemory(this);
		//RenderSystem::CreateVertexTriangle(this);
		RenderSystem::CreateVertexRectangle(this);
		//RenderSystem::CreateVertexBufferHost(this);
		RenderSystem::CreateVertexBufferStageing(this);
		RenderSystem::CreateIndexBufferHost(this);
		RenderSystem::CreateUniformBuffersHost(this);
		RenderSystem::CreateDescriptorPool(this);
		RenderSystem::CreateTextureImage(this);
		RenderSystem::CreateTextureImageView(this);
		RenderSystem::CreateTextureSampler(this);
		RenderSystem::AllocateDescriptorSets(this);
	}

	void Update() {
		RenderSystem::DrawFrame(this);
		RenderSystem::TryRecreateSwapchain(this);
	}

	void Destroy() {
		RenderSystem::WaitIdle(this);

		RenderSystem::DestroyTextureSampler(this);
		RenderSystem::DestroyTextureImageView(this);
		RenderSystem::DestroyTextureImage(this);
		RenderSystem::DestroyDescriptorPool(this);
		RenderSystem::DestroyUniformBuffers(this);
		RenderSystem::DestroyIndexBuffer(this);
		RenderSystem::DestroyVertexBuffer(this);
		RenderSystem::DestroyFences(this);
		RenderSystem::DestroySemaphores(this);
		RenderSystem::DestroyFrameBuffers(this);
		RenderSystem::DestroyGraphicsPipeline(this);
		RenderSystem::DestroyRenderPass(this);
		RenderSystem::DestroyCommandPool(this);
		RenderSystem::DestroyPipelineLayout(this);
		RenderSystem::DestroyDescriptorSetLayout(this);
		RenderSystem::DestroyAllShaders(this);
		RenderSystem::DestroySwapchianImageViews(this);
		RenderSystem::DestroySwapchian(this);
		RenderSystem::DestroyLogicDevice(this);
		RenderSystem::DestroySurface(this);
		//RenderSystem::DestroyDebugCallback(this);
		RenderSystem::DestroyInstance(this);
	}

	void OnSizeCallback() {
		RenderSystem::SetNeedRecreateSwapchain(this);
	}

};
