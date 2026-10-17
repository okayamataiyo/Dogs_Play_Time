//インクルード
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <chrono>
#include "Direct3D.h"
#include "Global.h"
#include "RootObject.h"
#include "Model.h"
#include "Image.h"
#include "Camera.h"
#include "Input.h"
#include "Audio.h"
#include "VFX.h"
#include "Debug.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
//ライブラリ
#pragma comment(lib,"Winmm.lib")

//定数宣言
const char* WIN_CLASS_NAME = "Dogs Play Time";	//ウィンドウクラス名

//プロトタイプ宣言
HWND InitApp(HINSTANCE hInstance, int screenWidth, int screenHeight, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void CheckSettingFiles();
bool IsInitSucceeded(HRESULT hr, const std::string& name);

// エントリーポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
//#if defined(DEBUG) | defined(_DEBUG)
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif

	srand((unsigned)time(nullptr));

	//ログファイルを開く（exeと同じ場所のLog.txt）
	Debug::OpenLogFile();

	//初期化できたものを覚えておく（失敗したときに、できたものだけを解放するため）
	bool isComInitialized = false;
	bool isImGuiContextCreated = false;
	bool isImGuiWin32Initialized = false;
	bool isImGuiDX11Initialized = false;
	HWND hWnd = nullptr;
	RootObject* pRootObject = nullptr;

	//初期化ファイルから取得する情報（取得できなかったときの既定値）
	int screenWidth = 800;		//スクリーンの幅
	int screenHeight = 600;		//スクリーンの高さ
	int fpsLimit = 60;			//FPS（画面更新速度）
	int isDrawFps = 0;			//キャプションに現在のFPSを表示するかどうか

	//起動の準備ができたか（falseのときはメッセージループに入らずに終了する）
	bool isReady = false;

	//準備の途中で失敗したら、breakで抜けて後始末へ進む
	do
	{
		//Assetsフォルダへ移動する（以降のファイルは、ここからの相対パスで読み込む）
		if (!SetCurrentDirectory("Assets"))
		{
			Debug::Fatal("Assetsフォルダが見つかりません。exeを正しい場所から起動してください。");
			break;
		}

		//設定ファイルが無いときは、既定値で動くことを記録しておく
		CheckSettingFiles();

		//初期化ファイルから必要な情報を取得
		screenWidth = GetPrivateProfileInt("SCREEN", "Width", 800, "Setting/SystemSetting.ini");		//スクリーンの幅
		screenHeight = GetPrivateProfileInt("SCREEN", "Height", 600, "Setting/SystemSetting.ini");	//スクリーンの高さ
		fpsLimit = GetPrivateProfileInt("GAME", "Fps", 60, "Setting/SystemSetting.ini");				//FPS（画面更新速度）
		isDrawFps = GetPrivateProfileInt("DEBUG", "ViewFps", 0, "Setting/SystemSetting.ini");		//キャプションに現在のFPSを表示するかどうか

		//COMを初期化する（WICやXAudio2などが使う。アプリケーションが動いている間ずっと有効にしておく）
		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (!IsInitSucceeded(hr, "COM"))	break;
		isComInitialized = true;

		//ウィンドウを作成
		hWnd = InitApp(hInstance, screenWidth, screenHeight, nCmdShow);
		if (hWnd == nullptr)	break;		//原因はInitAppの中で記録済み


		//Direct3D準備
		hr = Direct3D::Initialize(hWnd, screenWidth, screenHeight);
		if (!IsInitSucceeded(hr, "Direct3D"))	break;

		ImGui::CreateContext();
		isImGuiContextCreated = true;
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		style.ScaleAllSizes(3.0f); // すべてのサイズをスケーリング
		ImGui::StyleColorsDark();
		if (!ImGui_ImplWin32_Init(hWnd))
		{
			Debug::Fatal("ImGui(Win32)を初期化できません。");
			break;
		}
		isImGuiWin32Initialized = true;
		if (!ImGui_ImplDX11_Init(Direct3D::pDevice_, Direct3D::pContext_))
		{
			Debug::Fatal("ImGui(DirectX11)を初期化できません。");
			break;
		}
		isImGuiDX11Initialized = true;

		//カメラを準備
		Camera::Initialize();

		//入力処理（キーボード、マウス、コントローラー）の準備
		hr = Input::Initialize(hWnd);
		if (!IsInitSucceeded(hr, "入力処理"))	break;

		//オーディオ（効果音）の準備
		hr = Audio::Initialize();
		if (!IsInitSucceeded(hr, "オーディオ"))	break;


		//ルートオブジェクト準備
		//すべてのゲームオブジェクトの親となるオブジェクト
		pRootObject = new RootObject;
		pRootObject->Initialize();

		//最初のシーンで、必須のリソースの読み込みに失敗していたら先へ進まない
		if (Debug::HasFatal())	break;

		isReady = true;
	} while (false);

	bool isPause = false;


	//メッセージループ（何か起きるのを待つ）
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (isReady && msg.message != WM_QUIT)
	{
		//メッセージあり（こっちが優先）
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//メッセージなし（ここでゲームの処理）
		else
		{
			//時間計測
			timeBeginPeriod(1);	//時間計測の制度を上げる
			static int FPS = 0;								//画面更新回数のカウンタ
			static DWORD lastFpsResetTime = timeGetTime();	//最後にキャプションにFPSを表示した
			static DWORD lastUpdateTime = timeGetTime();	//最後に画面を更新した時間
			DWORD nowTime = timeGetTime();					//現在の時間

			//キャプションに現在のFPSを表示する
			if (isDrawFps)
			{
				//1秒（1000ミリ秒）経ったら
				if (nowTime - lastFpsResetTime > 1000)
				{
					//FPSの値を表示
					char string[16];
					wsprintf(string, "FPS:%d", FPS);
					SetWindowText(GetActiveWindow(), string);
					FPS = 0;
					lastFpsResetTime = nowTime;
				}
			}


			//指定した時間（FPSを60に設定した場合は60分の1秒）経過していたら更新処理
			if ((nowTime - lastUpdateTime) * fpsLimit > 1000.0f)
			{
				//時間計測関連
				lastUpdateTime = nowTime;	//現在の時間（最後に画面を更新した時間）を覚えておく
				FPS++;						//画面更新回数をカウントする
				Direct3D::FPS = FPS;

				if (Input::IsKeyDown(DIK_P))
				{
					isPause = !isPause;
				}

				//入力（キーボード、マウス、コントローラー）情報を更新
				Input::Update();

				//このフレームの描画開始
				Direct3D::BeginDraw();

				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				if (!isPause)
				{
					Direct3D::Update();

					//全オブジェクトの更新処理
					//ルートオブジェクトのUpdateを呼んだあと、自動的に子、孫のUpdateが呼ばれる
					pRootObject->UpdateSub();

					//エフェクトの更新
					VFX::Update();
				}
				else
				{
					while (::ShowCursor(true) < 0);
				}

				//必須のリソースの読み込みに失敗していたら、描画せずに終了へ向かう
				if (Debug::HasFatal())
				{
					PostQuitMessage(1);
				}

				for (int i = 0u; i < 3; i++) {
					Direct3D::SetViewPort(i);	//ビューポートにセット

					Camera::Update(i);			//カメラを更新
					if (!Debug::HasFatal())
					{
						//全オブジェクトを描画
						//ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
						pRootObject->DrawSub();

						//エフェクトの描画
						VFX::Draw();
					}

				}

				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
				//描画終了
				Direct3D::EndDraw();

				if (Input::IsKeyUp(DIK_ESCAPE))
				{
					PostQuitMessage(0);
				}
				//ちょっと休ませる
				Sleep(1);
			}
			timeEndPeriod(1);	//時間計測の制度を戻す
		}
	}



	//致命的なエラーが起きていたら、原因を知らせて異常終了にする
	int exitCode = 0;
	if (Debug::HasFatal())
	{
		if (hWnd != nullptr)	ShowWindow(hWnd, SW_HIDE);
		Debug::ShowFatal();
		exitCode = 1;
	}

	//いろいろ解放（初期化できたものだけ）
	VFX::Release();
	Audio::AllRelease();
	Model::AllRelease();
	Image::AllRelease();
	if (pRootObject != nullptr)
	{
		pRootObject->ReleaseSub();
		SAFE_DELETE(pRootObject);
	}
	Input::Release();
	if (isImGuiDX11Initialized)		ImGui_ImplDX11_Shutdown();
	if (isImGuiWin32Initialized)	ImGui_ImplWin32_Shutdown();
	if (isImGuiContextCreated)		ImGui::DestroyContext();
	Direct3D::Release();
	if (isComInitialized)			CoUninitialize();

	Debug::CloseLogFile();
	return exitCode;
}
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);


//ウィンドウの作成
HWND InitApp(HINSTANCE hInstance, int screenWidth, int screenHeight, int nCmdShow)
{
	//ウィンドウクラス（設計図）を作成
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);					//この構造体のサイズ
	wc.hInstance = hInstance;						//インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME;				//ウィンドウクラス名
	wc.lpfnWndProc = WndProc;						//ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW;				//スタイル（デフォルト）
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);	//アイコン
	wc.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);	//小さいアイコン
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);	//マウスカーソル
	wc.lpszMenuName = nullptr;						//メニュー（なし）
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//背景（白）
	if (RegisterClassEx(&wc) == 0)
	{
		Debug::Fatal("ウィンドウクラスを登録できません。" + Debug::HResult(HRESULT_FROM_WIN32(GetLastError())));
		return nullptr;
	}

	//ウィンドウサイズの計算
	RECT winRect = { 0, 0, screenWidth, screenHeight };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);

	//タイトルバーに表示する内容
	char caption[64];
	GetPrivateProfileString("SCREEN", "Caption", "***", caption, 64, "Setting/SystemSetting.ini");

	//ウィンドウを作成
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,					//ウィンドウクラス名
		caption,						//タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW,			//スタイル（普通のウィンドウ）
		CW_USEDEFAULT,					//表示位置左（おまかせ）
		CW_USEDEFAULT,					//表示位置上（おまかせ）
		winRect.right - winRect.left,	//ウィンドウ幅
		winRect.bottom - winRect.top,	//ウィンドウ高さ
		nullptr,						//親ウインドウ（なし）
		nullptr,						//メニュー（なし）
		hInstance,						//インスタンス
		nullptr							//パラメータ（なし）
	);
	if (hWnd == nullptr)
	{
		Debug::Fatal("ウィンドウを作成できません。" + Debug::HResult(HRESULT_FROM_WIN32(GetLastError())));
		return nullptr;
	}

	//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);

	return hWnd;
}


//ウィンドウプロシージャ（何かあった時によばれる関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	//ウィンドウを閉じた
	case WM_DESTROY:
		PostQuitMessage(0);	//プログラム終了
		return 0;

	//マウスが動いた
	case WM_MOUSEMOVE:
		Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


//設定ファイルが存在するか調べる
//無くても既定値で動くので、記録だけ残す
void CheckSettingFiles()
{
	const char* settingFiles[] =
	{
		"Setting/SystemSetting.ini",
		"Setting/PlaySceneSetting.ini",
		"Setting/PlayerSetting.ini",
		"Setting/ImguiSetting.ini",
	};

	for (const char* file : settingFiles)
	{
		if (GetFileAttributes(file) == INVALID_FILE_ATTRIBUTES)
		{
			Debug::Warning(std::string("設定ファイルが見つかりません。既定値で動作します: ") + file);
		}
	}
}

//初期化の結果を調べる
//失敗していて、まだ原因が記録されていなければ、ここで記録する
//引数：hr		初期化の戻り値
//引数：name	初期化したものの名前（記録用）
//戻値：成功したか
bool IsInitSucceeded(HRESULT hr, const std::string& name)
{
	if (SUCCEEDED(hr))	return true;

	if (!Debug::HasFatal())
	{
		Debug::Fatal(name + "を初期化できません。" + Debug::HResult(hr));
	}
	return false;
}
