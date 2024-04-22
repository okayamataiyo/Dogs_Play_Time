#include "Json.hpp"

namespace JsonReader
{
	// JSONファイルの読み込み
	void Load(const std::string& filename);

	// 指定したセクションを取得
	const nlohmann::json& GetSection(const std::string& key);
};