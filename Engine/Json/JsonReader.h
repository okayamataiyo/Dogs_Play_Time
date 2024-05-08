#include "Json.hpp"

namespace JsonReader
{
	// JSONファイルの読み込み
	void Load(const std::string& filename);

	//JSONファイルへの書き込み
	void Save(const std::string& _fileName, const nlohmann::json& _jsonData);

	// 指定したセクションを取得
	const nlohmann::json& GetSection(const std::string& key);
};