#pragma once

#include <shared/Exported.hxx>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
	EXPORTED void Physics_Init();
	EXPORTED void Physics_Update(float delta);
	EXPORTED uint64_t Physics_CreateBoxCollider(float x, float y, float width, float height);
	EXPORTED uint64_t Physics_CreateCircleCollider(float x, float y, float radius);
	EXPORTED uint64_t Physics_CreateCharacterController(
		float x,
		float y,
		float radius,
		float density,
		float friction,
		void* userData
	);
	EXPORTED void Physics_MoveCharacterController(uint64_t id, float x, float y);
	EXPORTED void Physics_SetCharacterControllerPosition(uint64_t id, float x, float y);
	EXPORTED void Physics_SetCharacterControllerAngle(uint64_t id, float angle);
#ifdef __cplusplus
}
#endif