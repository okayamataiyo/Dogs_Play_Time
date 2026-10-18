#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <string>

//テストで使う一時フォルダ（作るときに新しいフォルダを用意し、壊すときに中身ごと消す）
class TempDir
{
	std::filesystem::path path_;

public:
	TempDir()
	{
		static int count = 0;
		path_ = std::filesystem::temp_directory_path() /
			("dogs_play_time_tests_" + std::to_string(GetCurrentProcessId()) + "_" + std::to_string(count++));
		std::filesystem::create_directories(path_);
	}

	~TempDir()
	{
		std::error_code error;
		std::filesystem::remove_all(path_, error);
	}

	//フォルダ自身のパス
	std::string Path() const
	{
		return path_.string();
	}

	//フォルダの中のファイルのパス
	std::string Path(const std::string& fileName) const
	{
		return (path_ / fileName).string();
	}

	//ファイルを作る（バイト列をそのまま書く）
	void Write(const std::string& fileName, const std::string& bytes) const
	{
		std::ofstream file(path_ / fileName, std::ios::binary);
		file.write(bytes.data(), (std::streamsize)bytes.size());
	}
};
