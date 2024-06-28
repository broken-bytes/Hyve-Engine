#pragma once

#include "Shader.hxx"
#include "Texture.hxx"

#include <cereal/cereal.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include <glm/glm.hpp>

#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace kyanite::engine::rendering {
	class Material {
	public:
		bool isInstanced = false;
		std::map<ShaderType, std::shared_ptr<Shader>> shaders;
		std::map<std::string, std::shared_ptr<Texture>> textures;
		std::map<std::string, float> floats;
		std::map<std::string, uint32_t> ints;
		std::map<std::string, bool> bools;
		std::map<std::string, std::array<float, 2>> vec2s;
		std::map<std::string, std::array<float, 3>> vec3s;
		std::map<std::string, std::array<float, 4>> vec4s;

		Material(
			std::map<ShaderType, std::shared_ptr<Shader>> shaders
		) : shaders(shaders), 
			textures({}),
			floats({}),
			ints({}),
			bools({}),
			vec2s({}),
			vec3s({}),
			vec4s({}) {}

		virtual void Bind() = 0;

		virtual void SetBuiltins(glm::mat4 model, glm::mat4 view, glm::mat4 projection) = 0;
	};
}

