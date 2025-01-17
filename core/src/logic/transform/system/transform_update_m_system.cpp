
#include "logic/transform/transform_logic.h"
#include "logic/transform/transform_system.h"
#include "context.hpp"
#include "engine_object.hpp"

namespace Logic
{
    void TransformUpdateMSystem::Update(Context *context)
    {
        auto &allEOs = context->allEOs;
        for (const auto &eo : allEOs)
        {
            auto hasTransform = eo->HasComponent<Transform>();
            if (hasTransform)
                TransformLogic::UpdateModel(context, eo);
        }
    }
}