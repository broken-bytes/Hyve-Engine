#include "assets/AssetDatabase.hxx"
#include <core/Core.hxx>

#include <sqlite3.h>

namespace kyanite::editor {

	auto AssetTypeToString(AssetType type) -> std::string {
		std::string typeString = "";

		switch (type) {
		case AssetType::ANIMATION:
			typeString = "ANIMATION";
			break;
		case AssetType::MATERIAL:
			typeString = "MATERIAL";
			break;
		case AssetType::MESH:
			typeString = "MESH";
			break;
		case AssetType::MODEL:
			typeString = "MODEL";
			break;
		case AssetType::SHADER:
			typeString = "SHADER";
			break;
		case AssetType::SPRITE:
			typeString = "SPRITE";
			break;
		case AssetType::TERRAIN:
			typeString = "TERRAIN";
			break;
		case AssetType::TEXTURE:
			typeString = "TEXTURE";
			break;
		case AssetType::SOUND:
			typeString = "SOUND";
			break;
		case AssetType::WORLD:
			typeString = "WORLD";
			break;
		}

		return typeString;	
	}

	auto StringToAssetType(std::string typeStr) -> AssetType {
		AssetType type = AssetType::ANIMATION;

		if (typeStr == "ANIMATION") {
			type = AssetType::ANIMATION;
		}
		else if (typeStr == "MATERIAL") {
			type = AssetType::MATERIAL;
		}
		else if (typeStr == "MESH") {
			type = AssetType::MESH;
		}
		else if (typeStr == "MODEL") {
			type = AssetType::MODEL;
		}
		else if (typeStr == "SHADER") {
			type = AssetType::SHADER;
		}
		else if (typeStr == "SPRITE") {
			type = AssetType::SPRITE;
		}
		else if (typeStr == "TERRAIN") {
			type = AssetType::TERRAIN;
		}
		else if (typeStr == "TEXTURE") {
			type = AssetType::TEXTURE;
		}
		else if (typeStr == "SOUND") {
			type = AssetType::SOUND;
		}
		else if (typeStr == "WORLD") {
			type = AssetType::WORLD;
		}

		return type;
	}

	AssetDatabase::AssetDatabase() {

	}

	AssetDatabase::~AssetDatabase() {

	}

	auto AssetDatabase::AddAsset(std::string name, std::string path, AssetType type) -> std::string {
		// Generate a UUID for the asset
		auto uuid = kyanite::engine::core::CreateUUID();

		sqlite3_stmt* statement;
		sqlite3_prepare_v3(
			_database,
			"INSERT INTO assets (uuid, name, path, type) VALUES (?, ?, ?, ?)",
			-1,
			SQLITE_PREPARE_PERSISTENT,
			&statement,
			nullptr
		);
		sqlite3_bind_text(statement, 1, uuid.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(statement, 2, name.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(statement, 3, path.c_str(), -1, SQLITE_STATIC);
		auto typeStr = AssetTypeToString(type);
		sqlite3_bind_text(statement, 4, typeStr.c_str(), -1, SQLITE_STATIC);

		sqlite3_step(statement);
		sqlite3_finalize(statement);

		return uuid;
	}

	auto AssetDatabase::Load(std::filesystem::path path) -> void {
		auto dbPath = path / "assets.db";

		// Create database if it doesn't exist
		CreateDatabase(dbPath);

		if (sqlite3_open(dbPath.string().c_str(), &_database) != 0) {
			throw std::runtime_error("Failed to open database");
		}
	}

	auto AssetDatabase::LoadPackageList(std::string path)->std::vector<assetpackages::AssetPackage*> {
		return {};
	}

	auto AssetDatabase::CheckIfPackageHasAsset(const assetpackages::AssetPackage* package, std::string path) -> bool {
		return true;
	}

	auto AssetDatabase::LoadPackage(std::string path)->assetpackages::AssetPackage* {
		return nullptr;
	}

	auto AssetDatabase::LoadAsset(const assetpackages::AssetPackage* package, std::string path)->std::vector<uint8_t> {
		// TODO: Load from database
		return {};
	}

	auto AssetDatabase::CreateDatabase(std::filesystem::path path) -> void {
		if(!kyanite::engine::core::CheckIfFileExists(path.string())) {
			kyanite::engine::core::CreateFile(path.string());
		}

		if (sqlite3_open(path.string().c_str(), &_database) != 0) {
			throw std::runtime_error("Failed to open database");
		}

		// Create tables
		// Asset table consists of this:
		// - Asset UUID
		// - Asset Name
		// - Asset Type
		// - Asset Blob Path
		if (sqlite3_exec(
			_database,
			"CREATE TABLE IF NOT EXISTS assets (uuid TEXT PRIMARY KEY, name TEXT, updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, type TEXT, path TEXT)", nullptr, nullptr, nullptr) != 0) {
			throw std::runtime_error("Failed to create assets table");
		}

		// Asset Package table consists of this:
		// - Package UUID
		// - Package Name
		// - Package Path
		if (sqlite3_exec(
			_database,
			"CREATE TABLE IF NOT EXISTS asset_packages (uuid TEXT PRIMARY KEY, name TEXT, path TEXT)", nullptr, nullptr, nullptr) != 0) {
			throw std::runtime_error("Failed to create asset_packages table");
		}

		// Dependency table consists of this:
		// - Dependency UUID
		// - Asset UUID
		// - Dependency UUID
		if (sqlite3_exec(
			_database,
			"CREATE TABLE IF NOT EXISTS dependencies (uuid TEXT PRIMARY KEY, assetUuid TEXT, dependencyUuid TEXT, FOREIGN KEY(assetUuid) REFERENCES assets(uuid), FOREIGN KEY(dependencyUuid) REFERENCES assets(uuid))", nullptr, nullptr, nullptr) != 0) {
			throw std::runtime_error("Failed to create dependencies table");
		}

		// Labels table consists of this:
		// - Label UUID
		// - Label name
		// - Label color
		if (sqlite3_exec(
			_database,
			"CREATE TABLE IF NOT EXISTS labels (uuid TEXT PRIMARY KEY, name TEXT, color TEXT)", nullptr, nullptr, nullptr) != 0) {
			throw std::runtime_error("Failed to create labels table");
		}

		// Asset Labels table consists of this:
		// - Asset label UUID
		// - Asset UUID
		// - Label UUID
		if (sqlite3_exec(
			_database,
			"CREATE TABLE IF NOT EXISTS asset_labels (uuid TEXT PRIMARY KEY, assetUuid TEXT, labelUuid TEXT, FOREIGN KEY(assetUuid) REFERENCES assets(uuid), FOREIGN KEY(labelUuid) REFERENCES labels(uuid))", nullptr, nullptr, nullptr) != 0) {
			throw std::runtime_error("Failed to create asset_labels table");
		}

		sqlite3_close(_database);
	}
}