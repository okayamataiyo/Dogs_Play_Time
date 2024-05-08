//�C���N���[�h
#include <string>
#include <fstream>
#include <iostream>
#include "JsonReader.h"

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

    //��JSON�t�@�C���ւ̏�������
    void Save(const std::string& _fileName , json _jsonData)
    {
        std::ofstream ofs(_fileName);
        if (!ofs.is_open())
        {
            assert(false);
            return;
        }
        
        ofs.open(_fileName, ios::out);
        ofs << _jsonData..dump() << endl;
        ofs.close();
    }

    const nlohmann::json& GetSection(const std::string& key)
    {
        return data_[key];
    }
}