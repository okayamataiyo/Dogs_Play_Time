#include "Json.hpp"

namespace JsonReader
{
	// JSON�t�@�C���̓ǂݍ���
	void Load(const std::string& _filename);

	//JSON�t�@�C���ւ̏�������
	void Save(const std::string& _fileName, const nlohmann::json& _jsonData);

	// �w�肵���Z�N�V�������擾
	const nlohmann::json& GetSection(const std::string& _key);
};