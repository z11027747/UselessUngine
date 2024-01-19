
#include "editor/json/component_json.h"
#include "render/material/material_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    std::shared_ptr<Render::Material> ComponentJson<Render::Material>::From(Context *context, const json11::Json &j)
    {
        auto &imageNameJArr = j["imageNames"].array_items();
        auto &paramsJArr = j["params"].array_items();

        auto material = std::make_shared<Render::Material>();
        material->pipelineName = j["pipelineName"].string_value();
        material->imageNames.clear();
        for (const auto &imageNameJObj : imageNameJArr)
        {
            material->imageNames.push_back(imageNameJObj.string_value());
        }
        material->params.clear();
        for (const auto &paramsJOBj : paramsJArr)
        {
            auto &paramsJObjArr = paramsJOBj.array_items();
            auto paramsX = (float)paramsJObjArr.at(0).number_value();
            auto paramsY = (float)paramsJObjArr.at(1).number_value();
            auto paramsZ = (float)paramsJObjArr.at(2).number_value();
            auto paramsW = (float)paramsJObjArr.at(3).number_value();
            material->params.push_back(glm::vec4(paramsX, paramsY, paramsZ, paramsW));
        }
        material->isImageCube = j["isImageCube"].bool_value();
        material->castShadow = j["castShadow"].bool_value();

        return material;
    }

    template <>
    json11::Json ComponentJson<Render::Material>::To(Context *context,
                                                     std::shared_ptr<Render::Material> material)
    {
        auto &pipelineName = material->pipelineName;
        auto &imageNames = material->imageNames;
        auto &params = material->params;
        auto isImageCube = material->isImageCube;
        auto castShadow = material->castShadow;

        auto paramsJArr = json11::Json::array{};
        for (auto &param : params)
        {
            auto paramsJObjArr = json11::Json::array{param.x, param.y, param.z, param.w};
            paramsJArr.push_back(paramsJObjArr);
        }

        auto jObj = json11::Json::object{
            {"type", Type_Render_Material},
            {"pipelineName", pipelineName},
            {"imageNames", imageNames},
            {"params", paramsJArr},
            {"isImageCube", isImageCube},
            {"castShadow", castShadow}};

        return jObj;
    }
}