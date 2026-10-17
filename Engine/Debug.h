#pragma once

#include <string>

namespace Debug
{
	void Log(int value, bool isLineFeed = false);
	void Log(float value, bool isLineFeed = false);
	void Log(std::string value, bool isLineFeed = false);

	//ログファイルを開く（アプリケーションの起動時に一度だけ呼ぶ）
	//exeと同じ場所のLog.txtを、起動ごとに上書きして作る
	//開けなくてもエラーにはならない（ログファイルに何も書かれなくなるだけ）
	void OpenLogFile();

	//ログファイルを閉じる（アプリケーションの終了時に呼ぶ）
	void CloseLogFile();

	//続行できる問題（任意のリソースの読み込み失敗など）を記録する
	//出力ウィンドウとログファイルに書く。ダイアログは出さない
	//引数：message	記録する内容
	void Warning(std::string message);

	//続行できない問題（必須のリソースの読み込み失敗など）を記録する
	//出力ウィンドウとログファイルに書き、致命的なエラーとして覚えておく
	//引数：message	記録する内容
	void Fatal(std::string message);

	//致命的なエラーが記録されているか
	bool HasFatal();

	//記録した致命的なエラーをダイアログで表示する（記録がなければ何もしない）
	void ShowFatal();

	//HRESULTを記録用の文字列にする（例：HRESULT=0x80070003 (指定されたパスが見つかりません。)）
	//Windowsが説明文を持っているものは、それも添える
	//引数：hr	HRESULT（long型で受け取る）
	std::string HResult(long hr);
};