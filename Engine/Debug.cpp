#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <vector>
#include "Debug.h"

namespace
{
	//ログファイル（開けなかった場合はnullptrのまま）
	FILE* pLogFile_ = nullptr;

	//記録した致命的なエラー
	std::vector<std::string> fatalMessages_;

	//ダイアログに出す致命的なエラーの最大件数
	const size_t FATAL_DIALOG_MAX = 5;

	//ダイアログに出す1件あたりの最大バイト数（長いコンパイルエラーなどを切り詰める）
	const size_t FATAL_MESSAGE_MAX = 600;

	//文字列を最大バイト数で切り詰める（全角文字の途中では切らない）
	std::string Truncate(const std::string& text, size_t maxBytes)
	{
		if (text.size() <= maxBytes)	return text;

		size_t pos = 0;
		while (pos < maxBytes)
		{
			size_t charBytes = IsDBCSLeadByte((BYTE)text[pos]) ? 2 : 1;
			if (pos + charBytes > maxBytes)	break;
			pos += charBytes;
		}
		return text.substr(0, pos) + "...";
	}

	//ANSI（Shift-JIS）の文字列をUTF-8に変換する
	std::string AnsiToUtf8(const std::string& ansi)
	{
		if (ansi.empty())	return "";

		int wideLength = MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), (int)ansi.size(), nullptr, 0);
		if (wideLength <= 0)	return ansi;

		std::wstring wide(wideLength, L'\0');
		MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), (int)ansi.size(), &wide[0], wideLength);

		int utf8Length = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), wideLength, nullptr, 0, nullptr, nullptr);
		if (utf8Length <= 0)	return ansi;

		std::string utf8(utf8Length, '\0');
		WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), wideLength, &utf8[0], utf8Length, nullptr, nullptr);
		return utf8;
	}

	//ログファイルに1行書く（書けないときは何もしない）
	void WriteLogLine(const std::string& level, const std::string& message)
	{
		if (pLogFile_ == nullptr)	return;

		SYSTEMTIME t;
		GetLocalTime(&t);
		char stamp[32];
		sprintf_s(stamp, "%04d-%02d-%02d %02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);

		std::string utf8 = AnsiToUtf8(std::string("[") + stamp + "] [" + level + "] " + message + "\r\n");
		fwrite(utf8.data(), 1, utf8.size(), pLogFile_);

		//異常終了してもログが残るようにすぐ書き出す
		fflush(pLogFile_);
	}
}

void Debug::Log(int value, bool isLineFeed)
{
	char str[256];
	sprintf_s(str, "%d", value);
	Debug::Log(str, isLineFeed);
}

void Debug::Log(float value, bool isLineFeed)
{
	char str[256];
	sprintf_s(str, "%f", value);
	Debug::Log(str, isLineFeed);
}

void Debug::Log(std::string value, bool isLineFeed)
{
	if (isLineFeed)	value += "\n";
	OutputDebugString(value.c_str());
}

//ログファイルを開く
void Debug::OpenLogFile()
{
	if (pLogFile_ != nullptr)	return;

	//exeと同じ場所のLog.txt
	wchar_t path[MAX_PATH];
	DWORD length = GetModuleFileNameW(nullptr, path, MAX_PATH);
	if (length == 0 || length >= MAX_PATH)	return;

	wchar_t* pLastSeparator = wcsrchr(path, L'\\');
	if (pLastSeparator == nullptr)	return;
	*(pLastSeparator + 1) = L'\0';
	if (wcscat_s(path, MAX_PATH, L"Log.txt") != 0)	return;

	//起動ごとに上書きする
	if (_wfopen_s(&pLogFile_, path, L"wb") != 0)
	{
		pLogFile_ = nullptr;
		return;
	}

	//UTF-8のBOM（メモ帳などで文字化けしないようにする）
	const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
	fwrite(bom, 1, sizeof(bom), pLogFile_);

	WriteLogLine("情報", "ログを開始しました。");
}

//ログファイルを閉じる
void Debug::CloseLogFile()
{
	if (pLogFile_ == nullptr)	return;

	WriteLogLine("情報", "ログを終了しました。");
	fclose(pLogFile_);
	pLogFile_ = nullptr;
}

//続行できる問題を記録する
void Debug::Warning(std::string message)
{
	Debug::Log("[警告] " + message, true);
	WriteLogLine("警告", message);
}

//続行できない問題を記録する
void Debug::Fatal(std::string message)
{
	Debug::Log("[致命] " + message, true);
	WriteLogLine("致命", message);
	fatalMessages_.push_back(message);
}

//致命的なエラーが記録されているか
bool Debug::HasFatal()
{
	return !fatalMessages_.empty();
}

//記録した致命的なエラーをダイアログで表示する
void Debug::ShowFatal()
{
	if (fatalMessages_.empty())	return;

	std::string text = "続行できないエラーが起きたので、ゲームを終了します。\n\n";
	for (size_t i = 0; i < fatalMessages_.size() && i < FATAL_DIALOG_MAX; i++)
	{
		text += "・" + Truncate(fatalMessages_[i], FATAL_MESSAGE_MAX) + "\n";
	}
	if (fatalMessages_.size() > FATAL_DIALOG_MAX)
	{
		text += "ほか" + std::to_string(fatalMessages_.size() - FATAL_DIALOG_MAX) + "件\n";
	}
	text += "\n詳細は、exeと同じ場所のLog.txtを確認してください。";

	MessageBox(nullptr, text.c_str(), "Dogs Play Time - エラー", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
}