#pragma once

#include <memory>
#include <string>
#include <vector>

namespace kyanite::engine::io {
	struct FileHandle : std::enable_shared_from_this<FileHandle>{
		std::weak_ptr<FileHandle> Parent;
		std::vector<std::shared_ptr<FileHandle>> Children;
		std::string Name;
		std::string MountPoint;

		explicit FileHandle(std::string name) : Name(name) {}

		void AddChild(std::shared_ptr<FileHandle> child) {
			child->Parent = shared_from_this();
			Children.push_back(child);
		}

		// Find a child by name
		std::shared_ptr<FileHandle> FindChild(const std::string& name) {
			for (auto& child : Children) {
				if (child->Name == name) {
					return child;
				}
			}

			return nullptr;
		}
	};
}