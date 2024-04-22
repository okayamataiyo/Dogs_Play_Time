#include "JsonReader.h"

#include <string>
#include <fstream>

namespace JsonReader
{
    // JSONデータ
    nlohmann::json data_;

    // JSONファイルの読み込み
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