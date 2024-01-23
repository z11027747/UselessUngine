
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/material/material_logic.h"
#include "render/material/impl/material_light_model_logic.h"
#include "render/material/impl/material_color_logic.h"
#include "render/material/impl/material_shadow_logic.h"
#include "render/material/impl/material_skybox_logic.h"
#include "context.h"

namespace Render
{
    void MaterialPipelineLogic::SetVertexAttrDescriptions(Context *context,
                                                          std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &name = graphicsPipeline->name;
        if (name == Pipeline_Color)
        {
            MaterialColorPipelineLogic::SetVertexAttrDescriptions(context,
                                                                  graphicsPipeline);
        }
        else if (name == Pipeline_LightModel)
        {
            MaterialLightModelPipelineLogic::SetVertexAttrDescriptions(context,
                                                                       graphicsPipeline);
        }
        else if (name == Pipeline_Shadow)
        {
            MaterialShadowPipelineLogic::SetVertexAttrDescriptions(context,
                                                                   graphicsPipeline);
        }
        else if (name == Pipeline_Skybox)
        {
            MaterialSkyboxPipelineLogic::SetVertexAttrDescriptions(context,
                                                                   graphicsPipeline);
        }
    }
    void MaterialPipelineLogic::SetViewport(Context *context,
                                            std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &name = graphicsPipeline->name;
        if (name == Pipeline_Shadow)
        {
            MaterialShadowPipelineLogic::SetViewport(context,
                                                     graphicsPipeline);
        }
    }
    void MaterialPipelineLogic::SetRasterizationCreateInfo(Context *context,
                                                           std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &name = graphicsPipeline->name;
        if (name == Pipeline_Color)
        {
            MaterialColorPipelineLogic::SetRasterizationCreateInfo(context,
                                                                   graphicsPipeline);
        }
        else if (name == Pipeline_LightModel)
        {
            MaterialLightModelPipelineLogic::SetRasterizationCreateInfo(context,
                                                                        graphicsPipeline);
        }
        else if (name == Pipeline_Shadow)
        {
            MaterialShadowPipelineLogic::SetRasterizationCreateInfo(context,
                                                                    graphicsPipeline);
        }
        else if (name == Pipeline_Skybox)
        {
            MaterialSkyboxPipelineLogic::SetRasterizationCreateInfo(context,
                                                                    graphicsPipeline);
        }
    }
    void MaterialPipelineLogic::SetMultisampleCreateInfo(Context *context,
                                                         std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &name = graphicsPipeline->name;
        if (name == Pipeline_Color ||
            name == Pipeline_LightModel ||
            name == Pipeline_Skybox)
        {
            MaterialLightModelPipelineLogic::SetMultisampleCreateInfo(context,
                                                                      graphicsPipeline);
        }
    }
    void MaterialPipelineLogic::SetDepthStencilCreateInfo(Context *context,
                                                          std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &name = graphicsPipeline->name;
        if (name == Pipeline_Color)
        {
            MaterialColorPipelineLogic::SetDepthStencilCreateInfo(context,
                                                                  graphicsPipeline);
        }
        else if (name == Pipeline_Skybox)
        {
            MaterialSkyboxPipelineLogic::SetDepthStencilCreateInfo(context,
                                                                   graphicsPipeline);
        }
    }
    void MaterialPipelineLogic::SetColorBlendStage(Context *context,
                                                   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
    }
}