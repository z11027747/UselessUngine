
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/material/material_comp.h"
#include "render/material/material_logic.h"
#include "context.hpp"
#include "engine_object.hpp"

namespace Render
{
    void MaterialGlobalDescriptorLogic::Create(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();
        auto &logicalDevice = global->logicalDevice;
        auto swapchainImageCount = global->swapchainImageCount;

        // set layout
        VkDescriptorSetLayoutBinding globalUBO = {
            0,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            1,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT};

        std::vector<VkDescriptorSetLayoutBinding> globalBindings;
        globalBindings.push_back(globalUBO);

        auto globalDescriptorSetLayout = DescriptorSetLayoutLogic::Create(context, globalBindings);

        // descriptor
        auto globalDescriptor = std::make_shared<Descriptor>();

        auto globalDescriptorSet = DescriptorSetLogic::AllocateOne(context, globalDescriptorSetLayout);
        globalDescriptor->set = globalDescriptorSet;

        auto globalUBOSize = sizeof(GlobalUBO);
        auto globalBuffer = BufferLogic::Create(context,
                                                globalUBOSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        VkDescriptorBufferInfo bufferInfo = {
            globalBuffer->vkBuffer,
            0,
            globalBuffer->size};
        globalDescriptor->bufferInfos.push_back(bufferInfo);

        global->globalDescriptorSetLayout = globalDescriptorSetLayout;
        global->globalDescriptor = globalDescriptor;
        global->globalBuffer = globalBuffer;

        // update
        DescriptorSetLogic::Update(context,
                                   [=](std::vector<VkWriteDescriptorSet> &writes)
                                   {
                                       DescriptorSetLogic::WriteBuffer(writes, globalDescriptor->set, 0,
                                                                       VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, globalDescriptor->bufferInfos[0]);
                                   });

        auto maxMipLevels = 10;
        global->globalSamplerRepeat = SamplerLogic::Create(context, true, false, 0, maxMipLevels);
        global->globalSamplerClampLinear = SamplerLogic::Create(context, true, true, 0, maxMipLevels);
        global->globalSamplerClampNearest = SamplerLogic::Create(context, false, true, 0, maxMipLevels);
    }

    void MaterialGlobalDescriptorLogic::Destroy(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        DescriptorSetLayoutLogic::Destroy(context, global->globalDescriptorSetLayout);
        BufferLogic::Destroy(context, global->globalBuffer);

        SamplerLogic::Destroy(context, global->globalSamplerRepeat);
        SamplerLogic::Destroy(context, global->globalSamplerClampLinear);
        SamplerLogic::Destroy(context, global->globalSamplerClampNearest);
    }
}