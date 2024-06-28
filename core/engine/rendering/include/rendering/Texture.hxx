#pragma once

#include <glm/glm.hpp>

#include <cereal/access.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace kyanite::engine::rendering {
    struct Texture {
    public:
        uint32_t ID() const { return _id; }
    protected:
        uint32_t _id;
    };
}

