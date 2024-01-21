#pragma once

#include <string>
#include <vector>

namespace kyanite::engine::assetpackages {
	class AssetPackage;
	struct Metadata;

	class AssetPackageLoader {
	public:
		AssetPackageLoader();
		~AssetPackageLoader();

		auto LoadPackageList(std::string path) -> std::vector<AssetPackage*>;
		auto CheckIfPackageHasAsset(const AssetPackage* package, std::string path) -> bool;
		auto LoadPackage(std::string path) -> AssetPackage*;
		auto LoadAsset(const AssetPackage* package, std::string path) -> std::vector<uint8_t>;
	};
}