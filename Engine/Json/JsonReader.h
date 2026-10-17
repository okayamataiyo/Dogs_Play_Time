#include "Json.hpp"

namespace JsonReader
{
	// JSONファイルの読み込み
	// 戻値：成功→true　失敗→false（失敗した理由は警告として記録される。読み込み済みのデータは変わらない）
	bool Load(const std::string& _filename);

	//JSONファイルへの書き込み
	// 戻値：成功→true　失敗→false（失敗した理由は警告として記録される）
	bool Save(const std::string& _fileName, const nlohmann::json& _jsonData);

	// 指定したセクションを取得
	const nlohmann::json& GetSection(const std::string& _key);
};