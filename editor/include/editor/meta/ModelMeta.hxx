#pragma once

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>

#include <string>
#include <vector>

namespace kyanite::editor::meta {
	struct ModelMeta {
		std::string name;
		std::string uuid;
		std::vector<std::string> meshes;

	private:
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(name), CEREAL_NVP(uuid), CEREAL_NVP(meshes));
		}
	};
}