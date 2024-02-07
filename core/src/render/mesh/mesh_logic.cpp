
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    bool MeshLogic::IsShared(Context *context,
                             std::shared_ptr<Mesh> mesh)
    {
        return (mesh->info->vertexColor == glm::vec3(1.0f));
    }

    void MeshLogic::TryDestroyEO(Context *context,
                                 std::shared_ptr<EngineObject> eo)
    {
        auto mesh = eo->GetComponent<Render::Mesh>();
        if (!IsShared(context, mesh))
        {
            MeshInstanceLogic::SetDestroy(context, mesh->instance);
        }
    }
}