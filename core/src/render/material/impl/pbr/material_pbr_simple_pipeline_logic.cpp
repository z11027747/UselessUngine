
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_pbr_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	void MaterialPBRSimplePipelineLogic::SetVertexAttrDescriptions(Context *context,
																   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkVertexInputAttributeDescription positionOSDescription = {
			0, // location
			0, // binding
			VK_FORMAT_R32G32B32_SFLOAT,
			offsetof(Render::Vertex, positionOS)};
		VkVertexInputAttributeDescription normalOSDescription = {
			1, // location
			0, // binding
			VK_FORMAT_R32G32B32_SFLOAT,
			offsetof(Render::Vertex, normalOS)};
		VkVertexInputAttributeDescription colorDescription = {
			2, // location
			0, // binding
			VK_FORMAT_R32G32B32_SFLOAT,
			offsetof(Render::Vertex, color)};

		// // instancing
		// VkVertexInputAttributeDescription instancePosDescription = {
		// 	3, // location
		// 	1, // binding
		// 	VK_FORMAT_R32G32B32_SFLOAT,
		// 	offsetof(Render::VertexInstance, pos)};
		// VkVertexInputAttributeDescription instanceEulDescription = {
		// 	4, // location
		// 	1, // binding
		// 	VK_FORMAT_R32G32B32_SFLOAT,
		// 	offsetof(Render::VertexInstance, eul)};
		// VkVertexInputAttributeDescription instanceScaleDescription = {
		// 	5, // location
		// 	1, // binding
		// 	VK_FORMAT_R32_SFLOAT,
		// 	offsetof(Render::VertexInstance, scale)};

		auto &stageInfo = graphicsPipeline->stageInfo;
		stageInfo.vertexInputAttributeDescriptions = {
			positionOSDescription,
			normalOSDescription,
			colorDescription,
			// // instancing
			// instancePosDescription,
			// instanceEulDescription,
			// instanceScaleDescription
		};
	}
	void MaterialPBRSimplePipelineLogic::SetRasterizationCreateInfo(Context *context,
																	std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &rasterizationStateCreateInfo = stageInfo.rasterizationStateCreateInfo;
		rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	}
	void MaterialPBRSimplePipelineLogic::SetDepthStencilCreateInfo(Context *context,
																   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &depthStencilStateCreateInfo = stageInfo.depthStencilStateCreateInfo;
		depthStencilStateCreateInfo.depthTestEnable = true;
		depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depthStencilStateCreateInfo.depthWriteEnable = true;
	}
}