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
    void Save(const std::string& _fileName, const nlohmann::json& _jsonData)
    {
        // �t�@�C�����������݃��[�h�ŊJ��
        std::ofstream ofs(_fileName);

        // �t�@�C�����������J���Ȃ������ꍇ�̃G���[�n���h�����O
        if (!ofs.is_open())
        {
            std::cerr << "Failed to open file: " << _fileName << std::endl;
            return;
        }

        // JSON�f�[�^���t�@�C���ɏ�������
        ofs << _jsonData.dump() << std::endl;
    }

    const nlohmann::json& GetSection(const std::string& key)
    {
        return data_[key];
    }
}