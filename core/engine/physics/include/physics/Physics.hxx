#pragma once

#include <box2d/box2d.h>
#include <glm/glm.hpp>

namespace kyanite::engine::physics {
	void Init();
	void Update(float delta);
	uint64_t CreateBoxCollider(float x, float y, float width, float height);
	uint64_t CreateCircleCollider(float x, float y, float radius);
	uint64_t CreateCharacterController(
		float x,
		float y,
		float radius,
		float density,
		float friction,
		glm::mat4* transform
	);
	void MoveCharacterController(uint64_t id, float x, float y);
	void SetCharacterControllerPosition(uint64_t id, float x, float y);
	void SetCharacterControllerAngle(uint64_t id, float angle);
}