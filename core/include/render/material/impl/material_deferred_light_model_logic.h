#pragma once

#include <memory>
#include "render/material/material_comp.h"

class Context;

namespace Render
{
    class MaterialDeferredLightModelPipelineLogic final
    {
    public:
        static void SetVertexAttrDescriptions(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetRasterizationCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetMultisampleCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetDepthStencilCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetColorBlendStage(Context *, std::shared_ptr<GraphicsPipeline>);
    };
    class MaterialDeferredLightModelDescriptorLogic final
    {
    public:
        static void CreateSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
        static void AllocateAndUpdate(Context *, std::shared_ptr<MaterialData>);
        static void Destroy(Context *, std::shared_ptr<MaterialData>);
    };
}