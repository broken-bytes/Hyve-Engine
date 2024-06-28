#pragma once

#include <box2d/box2d.h>
#include <glm/glm.hpp>

namespace kyanite::engine::physics {
    class CharacterController {
    public:
        CharacterController(
            b2World* world,
            float x,
            float y,
            float radius,
            float density,
            float friction,
            glm::mat4* transform
        ) {
            // Define the dynamic body for the character
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(x, y);
            _body = world->CreateBody(&bodyDef);

            // Define the shape of the character (a box)
            b2CircleShape circleShape;
            circleShape.m_radius = radius;

            // Define the fixture for the character
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &circleShape;
            fixtureDef.density = density;
            fixtureDef.friction = friction;
            _body->CreateFixture(&fixtureDef);

            // Set the transform
            _body->SetTransform(b2Vec2(x, y), 0);

            fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(transform);
        }

        void SetTransform(float x, float y) {
			_body->SetTransform(b2Vec2(x, y), _body->GetAngle());
		}

        void SetAngle(float angle) {
			_body->SetTransform(_body->GetPosition(), angle);
		}

        void Move(float x, float y) {
			_body->SetLinearVelocity(b2Vec2(x, y));
		}

    private:

        b2Body* _body;
    };
}