#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <json/json11.hpp>
#include "logic/transform/transform_comp.h"

class Context;
class EngineObject;

namespace Logic
{
	class TransformLogic final
	{
	public:
		static void Add(std::shared_ptr<EngineObject>,
						const glm::vec3 &, const glm::vec3 &);
		static void Add(std::shared_ptr<EngineObject>,
						const glm::vec3 &, const glm::vec3 &, const glm::vec3 &);
		static void SetScale(std::shared_ptr<EngineObject>, const glm::vec3 &);
		static void SetScale(std::shared_ptr<EngineObject>, float);
		static void ResetAll(std::shared_ptr<EngineObject>);

		static void UpdateModel(Context *, std::shared_ptr<EngineObject>);

		static void SetParent(std::shared_ptr<EngineObject>,
							  std::shared_ptr<EngineObject>);

		static float Distance(std::shared_ptr<EngineObject>, std::shared_ptr<EngineObject>);
	};

	class TransformJson final
	{
	public:
		static std::shared_ptr<void> Parse(const json11::Json &);
		static json11::Json Serialize(std::shared_ptr<void>);
	};

	// Move =====================================
	class MoveLogic final
	{
	public:
		static void OnAdd(Context *, std::shared_ptr<EngineObject>);
		static void OnRemove(Context *, std::shared_ptr<EngineObject>);

		static void BeginFollow(Context *,
								std::shared_ptr<EngineObject>,
								std::shared_ptr<EngineObject>, const glm::vec3 &);
	};

	// Rotate =====================================
	class RotateLogic final
	{
	public:
		static void OnAdd(Context *, std::shared_ptr<EngineObject>);
		static void OnRemove(Context *, std::shared_ptr<EngineObject>);

		static void BeginRotateAround(Context *,
									  std::shared_ptr<EngineObject>,
									  std::shared_ptr<EngineObject>, float);
		static void BeginRotateAround(Context *,
									  std::shared_ptr<EngineObject>,
									  std::shared_ptr<EngineObject>, const glm::vec3 &, float);
	};

	// Scale =====================================
	class ScaleLogic final
	{
	public:
		static void OnAdd(Context *, std::shared_ptr<EngineObject>);
		static void OnRemove(Context *, std::shared_ptr<EngineObject>);

		static void BeginRotateAround(Context *,
									  std::shared_ptr<EngineObject>,
									  std::shared_ptr<EngineObject>, float);
	};
}