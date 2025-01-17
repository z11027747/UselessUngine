#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include "render/vk/buffer/buffer_comp.h"

namespace Render
{
    struct Vertex final
    {
        alignas(16) glm::vec3 positionOS;
        alignas(16) glm::vec3 normalOS;
        alignas(16) glm::vec3 tangentOS;
        alignas(16) glm::vec3 color;
        alignas(8) glm::vec2 uv0;

        bool operator==(const Vertex &other) const
        {
            return (positionOS == other.positionOS) &&
                   (normalOS == other.normalOS) &&
                   (tangentOS == other.tangentOS) &&
                   (color == other.color) &&
                   (uv0 == other.uv0);
        }
    };

    struct VertexInstance final
    {
        alignas(16) glm::vec3 pos;
        alignas(16) glm::vec3 eul;
        alignas(4) float scale{1.0f};
    };

    struct BoundingSphere final
    {
        glm::vec3 center;
        float radius;
    };

    struct MeshInfo final
    {
        std::string objName;
        glm::vec3 vertexColor{1.0f, 1.0f, 1.0f};
        bool checkHit{false};

        bool hasChanged{false};
    };

    struct MeshData final
    {
        int id;
        std::shared_ptr<MeshInfo> info; // ref
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;
        std::shared_ptr<Buffer> vertexBuffer;
        std::shared_ptr<Buffer> indexBuffer;
        BoundingSphere boundingSphere;
    };

    struct Mesh final
    {
        inline static std::string type{"Render::Mesh"};

        std::shared_ptr<MeshInfo> info{std::make_shared<MeshInfo>()};
        std::shared_ptr<MeshData> data;
    };

    struct MeshCache final
    {
        inline static std::string type{"Render::MeshCache"};

        std::unordered_map<std::string, std::shared_ptr<MeshData>> sharedMap{};
        std::vector<std::shared_ptr<MeshData>> deletes{};

		// instance
        std::vector<VertexInstance> vertexInstances{};
        std::shared_ptr<Buffer> vertexInstanceBuffer;
        const uint32_t maxInstanceCount = 1024;
        uint32_t currInstance;
    };
}
