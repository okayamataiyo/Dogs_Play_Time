#include "Json.hpp"

namespace JsonReader
{
	// JSON�t�@�C���̓ǂݍ���
	void Load(const std::string& filename);

	// �w�肵���Z�N�V�������擾
	const nlohmann::json& GetSection(const std::string& key);
};