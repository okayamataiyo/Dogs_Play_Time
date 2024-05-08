//インクルード
#include <string>
#include <fstream>
#include <iostream>
#include "JsonReader.h"

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

    //▼JSONファイルへの書き込み
    void Save(const std::string& _fileName, const nlohmann::json& _jsonData)
    {
        // ファイルを書き込みモードで開く
        std::ofstream ofs(_fileName);

        // ファイルが正しく開けなかった場合のエラーハンドリング
        if (!ofs.is_open())
        {
            std::cerr << "Failed to open file: " << _fileName << std::endl;
            return;
        }

        // JSONデータをファイルに書き込む
        ofs << _jsonData.dump() << std::endl;
    }

    const nlohmann::json& GetSection(const std::string& key)
    {
        return data_[key];
    }
}