﻿
#include "render/comp.h"
#include "context.h"

//内存
//Instance域函数中主要在 CPU 能够访问的（主板上）内存中进行分配和访问
//		该部分内存属于特殊的 Host 端内存
//Deivce域函数中主要在 GPU 能够访问的内存（显存）中进行内存分配和访问 
//		该部分内存属于特殊的 Device 端内存

//cpu 创建 host内存（使用new）
//拷贝到 host和device 都可以访问的内存 （对CPU友好，对GPU不友好）
//通过总线拷贝到 只有device 可以访问的内存

//VkPhysicalDeviceMemoryProperties  内存信息
//	memoryTypeCount 内存类型的数量
//	memoryTypes 对应的内存类型信息数据
//	memoryHeapCount 内存堆的数量
//	memoryHeaps 对应的内存堆的信息数据

//VkMemoryType	 内存类型
//	propertyFlags 该类内存的属性信息，使用标志位存储相应信息
//	heapIndex 对应的内存堆的索引，表示指向 memoryHeaps[heapIndex] 的内存堆

//VkMemoryPropertyFlagBits  内存标志位
//	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT 表示这部分内存为 GPU 物理设备自身的内存只有物理设备自身可访问，也就是 Device 端内存。
//	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT 表示这部分内存为 Host 端可访问到的内存只有 Host 端自身可访问， Device 端不可访问。
//	VK_MEMORY_PROPERTY_HOST_COHERENT_BIT 表示这部分内存为 Host 端连续内存，表示对于该内存的读写可连续进行（就像 CPU 对于内存的修改那样）。该内存类型不需要手动进行 刷新 和 失效 操作。
//	VK_MEMORY_PROPERTY_HOST_CACHED_BIT 表示这部分内存为 Host 端高速内存，并且自带 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT 属性。这一部分内存大小相对较小。
//	VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT 表示这部分内存为可以滞后分配内存，等要使用时再分配内存。
//	VK_MEMORY_PROPERTY_PROTECTED_BIT 表示这部分内存为受保护内存，并且只允许 GPU 硬件设备和受保护的队列（ VK_QUEUE_PROTECTED_BIT ）可以访问该内存。

//VkMemoryHeap	内存堆
//	size 表示该内存堆的大小。单位为字节
//	flags 表示该堆的属性标志位

//VkMemoryHeapFlags              
//	VK_MEMORY_HEAP_DEVICE_LOCAL_BIT 表示该内存堆为 GPU 专属内存
//	VK_MEMORY_HEAP_MULTI_INSTANCE_BIT 由于逻辑设备可以包含多个物理设备，此标志位表示该堆对应多个物理设备上的内存堆，对该堆的操作将会在每个物理设备的内存堆上进行相同的操作。

void RenderSystem::CheckPhysicalDeviceMemory(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& physicalDevice = globalInfoComp->physicalDevice;

	VkPhysicalDeviceMemoryProperties memoryProerties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProerties);

	for (auto i = 0; i < memoryProerties.memoryTypeCount; i++) {
		auto memoryType = memoryProerties.memoryTypes[i];

		std::cout << "=== >index: " << i << std::endl;
		if (memoryType.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
			std::cout << "Supprt DEVICE_LOCAL" << std::endl;
		}
		if (memoryType.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
			std::cout << "Supprt HOST_VISIBLE" << std::endl;
		}
		if (memoryType.propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
			std::cout << "Supprt HOST_COHERENT" << std::endl;
		}

		auto heapIndex = memoryType.heapIndex;
		auto memoryHeap = memoryProerties.memoryHeaps[heapIndex];

		std::cout << "heap index: " << heapIndex << ":" << (memoryHeap.size) / (1024 * 1024 * 1024) << " GB" << std::endl;
		if (memoryHeap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
			std::cout << "\t DEVICE_LOCAL" << std::endl;
		}
	}
}

//显卡可以分配不同类型的内存作为缓冲使用。
//	不同类型的内存所允许进行的操作以及操作的效率有所不同
//	我们需要结合自己的需求选择最合适的内存类型使用。
uint32_t RenderSystem::FindMemoryType(Context* context,
	uint32_t typeFilter, VkMemoryPropertyFlags propertiesFlags)
{
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& physicalDevice = globalInfoComp->physicalDevice;

	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (auto i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i))
			&& (memProperties.memoryTypes[i].propertyFlags & propertiesFlags) == propertiesFlags) {
			return i;
		}
	}

	throw std::runtime_error("failed find memory type!");
}

void RenderSystem::CreateBuffer(Context* context,
	//buffer 
	VkDeviceSize size, VkBufferUsageFlags usageFlags,
	//memory
	VkMemoryPropertyFlags propertiesFlags,
	VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& physicalDevice = globalInfoComp->physicalDevice;

	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	//createInfo.flags = 0; //用于配置缓冲的内存稀疏程度
	createInfo.size = size; //指定要创建的缓冲所占字节大小
	createInfo.usage = usageFlags; //缓冲中的数据的使用目的
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	auto bufferRet = vkCreateBuffer(logicDevice, &createInfo, nullptr, &buffer);
	if (bufferRet != VK_SUCCESS) {
		throw std::runtime_error("create vertexBuffer error!");
	}

	//缓冲（Buffer）创建后需要给它分配任何内存
	//	vkGetBufferMemoryRequirements函数获取缓冲的内存需求
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(logicDevice, buffer, &memRequirements);

	auto memoryTypeIndex = FindMemoryType(context,
		memRequirements.memoryTypeBits, propertiesFlags);

	VkMemoryAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.allocationSize = memRequirements.size;
	allocateInfo.memoryTypeIndex = memoryTypeIndex;

	auto ret = vkAllocateMemory(logicDevice, &allocateInfo, nullptr, &bufferMemory);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("allocate bufferMemory error!");
	}

	//绑定内存
	auto bindRet = vkBindBufferMemory(logicDevice, buffer, bufferMemory, 0);
	if (bindRet != VK_SUCCESS) {
		throw std::runtime_error("bind vertexMemory error!");
	}
}

void RenderSystem::DestroyBuffer(Context* context,
	VkBuffer& buffer, VkDeviceMemory& deviceMemory) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	vkDestroyBuffer(logicDevice, buffer, nullptr);
	vkFreeMemory(logicDevice, deviceMemory, nullptr);
}

void RenderSystem::CopyBuffer(Context* context,
	VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size)
{
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& logicQueue = globalInfoComp->logicQueue;

	std::vector<VkCommandBuffer> commandBuffers(1);
	AllocateCommandBuffer(context, commandBuffers);

	auto& commandBuffer = commandBuffers[0];

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	auto beginRet = vkBeginCommandBuffer(commandBuffer, &beginInfo);
	if (beginRet != VK_SUCCESS) {
		throw std::runtime_error("begin commandBuffer error!");
	}

	VkBufferCopy copy = {};
	copy.srcOffset = 0;
	copy.dstOffset = 0;
	copy.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copy);

	auto endRet = vkEndCommandBuffer(commandBuffer);
	if (endRet != VK_SUCCESS) {
		throw std::runtime_error("end commandBuffer error!");
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	auto submitRet = vkQueueSubmit(logicQueue, 1, &submitInfo, nullptr);
	if (submitRet != VK_SUCCESS) {
		throw std::runtime_error("submit error!");
	}

	//直接等待传输操作完成
	vkQueueWaitIdle(logicQueue);

	FreeCommandBuffer(context, commandBuffers);
}