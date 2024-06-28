#include "physics/Physics.hxx"
#include "physics/CharacterController.hxx"
#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <memory>

namespace kyanite::engine::physics {

	std::unique_ptr<b2World> world = nullptr;
	std::map<uint64_t, b2Body*> bodies = {};
	std::map<uint64_t, b2Fixture*> fixtures = {};
	std::map<uint64_t, CharacterController*> controllers = {};
	
	void Init() {
		world = std::make_unique<b2World>(b2Vec2 { 0, 0 });
	}

	void Update(float delta) {
		world->Step(delta, 8, 3);

		for (auto body = world->GetBodyList(); body != nullptr; body = body->GetNext()) {
			auto position = body->GetPosition();
			auto angle = body->GetAngle();

			// Update the transform of the entity
			auto data = body->GetUserData();
			// Turn it into a glm::mat4
			auto transform = (glm::mat4&)data;
			transform = glm::translate(transform, glm::vec3(position.x, position.y, 0));
			transform = glm::rotate(transform, angle, glm::vec3(0, 0, 1));
		}
	}

	uint64_t CreateBoxCollider(float x, float y, float width, float height) {
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(x, y);
		auto body = world->CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.SetAsBox(width / 2, height / 2);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		auto fixture = body->CreateFixture(&fixtureDef);

		auto id = (uint64_t)body;
		bodies[id] = body;
		fixtures[id] = fixture;

		return id;
	}

	uint64_t CreateCircleCollider(float x, float y, float radius) {
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(x, y);
		auto body = world->CreateBody(&bodyDef);

		b2CircleShape shape;
		shape.m_radius = radius;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		auto fixture = body->CreateFixture(&fixtureDef);

		auto id = (uint64_t)body;
		bodies[id] = body;
		fixtures[id] = fixture;

		return id;
	}

	uint64_t CreateCharacterController(
		float x, 
		float y, 
		float radius, 
		float density,
		float friction, 
		glm::mat4* transform
	) {
		auto controller = new CharacterController(world.get(), x, y, radius, density, friction, transform);
		auto id = (uint64_t)controller;
		controllers[id] = controller;
		return id;
	}

	void MoveCharacterController(uint64_t id, float x, float y) {
		auto controller = controllers[id];
		controller->Move(x, y);
	}

	void SetCharacterControllerPosition(uint64_t id, float x, float y) {
		auto controller = controllers[id];
		controller->SetTransform(x, y);
	}

	void SetCharacterControllerAngle(uint64_t id, float angle) {
		auto controller = controllers[id];
		controller->SetAngle(angle);
	}
}