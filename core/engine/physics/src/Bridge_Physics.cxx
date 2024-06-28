#include "physics/Bridge_Physics.h"
#include "physics/Physics.hxx"

using namespace kyanite::engine;

void Physics_Init() {
	physics::Init();
}

void Physics_Update(float delta) {
	physics::Update(delta);
}

uint64_t Physics_CreateBoxCollider(float x, float y, float width, float height) {
	return physics::CreateBoxCollider(x, y, width, height);
}

uint64_t Physics_CreateCircleCollider(float x, float y, float radius) {
	return physics::CreateCircleCollider(x, y, radius);
}

uint64_t Physics_CreateCharacterController(
	float x,
	float y,
	float radius,
	float density,
	float friction,
	void* userData
) {
	return physics::CreateCharacterController(
		x, 
		y, 
		radius, 
		density, 
		friction, 
		reinterpret_cast<glm::mat4*>(userData)
	);
}

void Physics_MoveCharacterController(uint64_t id, float x, float y) {
	physics::MoveCharacterController(id, x, y);
}

void Physics_SetCharacterControllerPosition(uint64_t id, float x, float y) {
	physics::SetCharacterControllerPosition(id, x, y);
}

void Physics_SetCharacterControllerAngle(uint64_t id, float angle) {
	physics::SetCharacterControllerAngle(id, angle);
}
