#pragma once


#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>

#include <glm/glm.hpp>

namespace kyanite::engine::rendering {
	struct Vertex {
		glm::vec3 position;
		glm::vec2 uvs;

	};
}
