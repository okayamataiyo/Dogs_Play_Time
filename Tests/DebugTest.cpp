#include "TestUtil.h"
#include <sstream>
#include "doctest.h"
#include "Debug.h"

//Log.txtは日本語の文を含み、その文字コード変換は実行する環境の言語設定に左右される。
//そのためここでは、英数字だけのメッセージで確かめる。
namespace
{
	//実行ファイルと同じ場所のLog.txtの中身
	std::string ReadLogFile()
	{
		char path[MAX_PATH];
		GetModuleFileNameA(nullptr, path, MAX_PATH);
		std::string folder = path;
		folder = folder.substr(0, folder.find_last_of('\\') + 1);

		std::ifstream file(folder + "Log.txt", std::ios::binary);
		std::stringstream stream;
		stream << file.rdbuf();
		return stream.str();
	}

	size_t Count(const std::string& text, const std::string& word)
	{
		size_t count = 0;
		for (size_t pos = text.find(word); pos != std::string::npos; pos = text.find(word, pos + word.size()))
		{
			count++;
		}
		return count;
	}
}

TEST_CASE("Debug::HResult: starts with the hex code, and adds the description Windows has")
{
	CHECK(Debug::HResult(0).rfind("HRESULT=0x00000000", 0) == 0);
	CHECK(Debug::HResult((long)0x80004005).rfind("HRESULT=0x80004005", 0) == 0);

	//0x8007xxxxはWindowsのエラーコード由来なので、説明文が付く（ERROR_FILE_NOT_FOUND）
	std::string text = Debug::HResult((long)0x80070002);
	CHECK(text.rfind("HRESULT=0x80070002", 0) == 0);
	CHECK(text.find(" (") != std::string::npos);
	CHECK(text.back() == ')');
}

TEST_CASE("Debug: the log file is made beside the exe with a BOM, and is overwritten every time it is opened")
{
	Debug::OpenLogFile();
	Debug::Warning("first-warning-ascii");
	Debug::CloseLogFile();

	std::string log = ReadLogFile();
	REQUIRE(log.size() > 3);
	CHECK((unsigned char)log[0] == 0xEF);
	CHECK((unsigned char)log[1] == 0xBB);
	CHECK((unsigned char)log[2] == 0xBF);
	CHECK(log.find("first-warning-ascii") != std::string::npos);
	CHECK(log.find("\r\n") != std::string::npos);

	Debug::OpenLogFile();
	Debug::Warning("second-warning-ascii");
	Debug::CloseLogFile();

	log = ReadLogFile();
	CHECK(log.find("first-warning-ascii") == std::string::npos);
	CHECK(log.find("second-warning-ascii") != std::string::npos);
}

TEST_CASE("Debug: recording without an open log file is harmless")
{
	Debug::CloseLogFile();
	Debug::Warning("warning-without-log-file");
	CHECK_FALSE(Debug::HasFatal());
}

//Fatalで覚えた状態は取り消せないので、HasFatalを調べるテストは、このテストより前に置くこと
TEST_CASE("Debug: Warning is never fatal, Fatal is, and every Fatal reaches the log")
{
	Debug::OpenLogFile();
	CHECK_FALSE(Debug::HasFatal());

	Debug::Warning("warning-ascii");
	CHECK_FALSE(Debug::HasFatal());

	Debug::Fatal("fatal-ascii");
	Debug::Fatal("fatal-ascii");
	CHECK(Debug::HasFatal());
	Debug::CloseLogFile();

	std::string log = ReadLogFile();
	CHECK(Count(log, "warning-ascii") == 1);
	CHECK(Count(log, "fatal-ascii") == 2);
}
