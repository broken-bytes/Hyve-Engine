#pragma once

// A handle that can hold anything. Used to pass references to the engine without exposing the actual type.
// Especially usefull for RAII types, like std::unique_ptr or std::shared_ptr.
struct Handle {
    void* Ptr;
};