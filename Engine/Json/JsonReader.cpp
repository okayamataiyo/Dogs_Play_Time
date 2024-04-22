#include "JsonReader.h"

#include <string>
#include <fstream>

namespace JsonReader
{
    // JSON�f�[�^
    nlohmann::json data_;

    // JSON�t�@�C���̓ǂݍ���
    void Load(const std::string& fileName)
    {

        std::ifstream ifs(fileName);
        if (!ifs.is_open())
        {
            assert(false);
            return;
        }

        nlohmann::json j;
        ifs >> j;
        if (j.empty())
        {
            assert(false);
            return;
        }

        data_ = j;
    }

    const nlohmann::json& GetSection(const std::string& key)
    {
        return data_[key];
    }
}