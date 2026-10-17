//インクルード
#include <string>
#include <fstream>
#include <iostream>
#include "JsonReader.h"
#include "../Debug.h"

namespace JsonReader
{
    // JSONデータ
    nlohmann::json data_;

    // JSONファイルの読み込み
    bool Load(const std::string& _fileName)
    {

        std::ifstream ifs(_fileName);
        if (!ifs.is_open())
        {
            Debug::Warning("JSONファイルを開けません: " + _fileName);
            return false;
        }

        // JSONとして読めない（書き方が間違っている）ときは例外が投げられるので、受け止めて失敗にする
        nlohmann::json j;
        try
        {
            ifs >> j;
        }
        catch (const nlohmann::json::exception& e)
        {
            Debug::Warning("JSONファイルを読み込めません: " + _fileName + " (" + e.what() + ")");
            return false;
        }

        if (j.empty())
        {
            Debug::Warning("JSONファイルの中身が空です: " + _fileName);
            return false;
        }

        data_ = j;
        return true;
    }

    //▼JSONファイルへの書き込み
    bool Save(const std::string& _fileName, const nlohmann::json& _jsonData)
    {
        // ファイルを書き込みモードで開く
        std::ofstream ofs(_fileName);

        // ファイルが正しく開けなかった場合のエラーハンドリング
        if (!ofs.is_open())
        {
            Debug::Warning("JSONファイルを書き込めません: " + _fileName);
            return false;
        }

        // JSONデータをファイルに書き込む
        ofs << _jsonData.dump() << std::endl;
        return !ofs.fail();
    }

    const nlohmann::json& GetSection(const std::string& _key)
    {
        return data_[_key];
    }
}