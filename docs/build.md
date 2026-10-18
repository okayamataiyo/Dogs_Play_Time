# ビルドとテスト

このリポジトリのビルド・実行・テストの手順と、必要な環境をまとめる。決まった経緯は`docs/adr/`のADRを参照する。

## 必要な環境

| 用途 | 必要なもの |
|------|------------|
| ゲーム本体・テスト | Windows 10 / 11（x64）。Visual Studio 2026（バージョン18）の「C++によるデスクトップ開発」（MSVCツールセットv145とWindows SDKを含む） |
| ゲーム本体だけ | FBX SDK 2020.3.7（VS2019用）。`C:\Program Files\Autodesk\FBX\FBX SDK\2020.3.7\`に入れる |

- ツールセットはv145（VS 2026）に固定している（`Dogs_Play_Time.vcxproj`と`Tests/Dogs_Play_Time.Tests.vcxproj`）。VS 2022（v143）でビルドするときは、手元だけでツールセットを変え、そのままコミットしないこと。
- FBX SDKはAutodeskのサイトから個別に入れる。ライセンス上、リポジトリには同梱しない（ADR-0005）。vcxprojは上の場所を絶対パスで参照している。
- FBX SDKを使うのはゲーム本体だけ。テスト（`Tests/`）は、FBX SDKがなくてもビルドして実行できる。

## ゲーム本体のビルドと実行

1. `Dogs_Play_Time.sln`をVisual Studioで開く。
2. 構成を`Debug`か`Release`にしてビルドする（プラットフォームは`x64`だけ）。出力は`x64\Debug\`か`x64\Release\`。ソリューションのビルドには、テストのプロジェクトも含まれる。
3. 実行するときのカレントディレクトリは、`Assets`フォルダがある場所（リポジトリの直下）にする。Visual Studioから実行すればそうなる。PowerShellなら、リポジトリの直下で`.\x64\Debug\Dogs_Play_Time.exe`を実行する。
4. 起動のたびに、exeと同じ場所に`Log.txt`ができる（警告と致命的なエラーの記録。ADR-0007）。

## 自動テスト

`Tests/`のプロジェクトが、ゲーム本体のソースのうち、FBX SDKとDirect3Dの初期化に依存しない部分をそのままコンパイルして確かめる。枠組みはdoctestで、ヘッダーを`Tests/third_party/doctest/`に置いている。

- Visual Studio: `Dogs_Play_Time.Tests`をビルドして、できたexeを実行する。
- コマンドライン（CIと同じ）:

  ```
  msbuild Tests\Dogs_Play_Time.Tests.vcxproj /p:Configuration=Debug /p:Platform=x64
  .\Tests\x64\Debug\Dogs_Play_Time.Tests.exe
  ```

  失敗したテストがあると、終了コードが0以外になる。一部のテストだけ流すときは、doctestのオプションを使う（例：`-tc="*GameObject*"`。一覧は`--help`）。
- 出力は`Tests\x64\<構成>\`にできる（`Log.txt`もここ）。ゲーム本体の`x64\`とは混ざらない。

### テストを足すとき

- `Tests/`に`.cpp`を作り、`Dogs_Play_Time.Tests.vcxproj`に加える。
- 確かめたいゲーム側のソースも、同じvcxprojに加える。`Model`と`Direct3D`の関数を呼ぶものは、`Tests/Stubs.cpp`のダミーでリンクできる。足りない関数があれば、本物と同じ宣言でダミーを足す。
- `GameObject`を使うテストでは、`Tests/TestObjects.h`の`TestObject`などを使うと、呼ばれた順序を記録できる。
- FBX SDKに依存するヘッダー（`Fbx.h`、`FbxParts.h`）は、テストのコンパイルに入れない。`Model.h`は`Fbx`を前方宣言するだけにして、読み込まないようにしてある。

### 文字コード

`Engine/`と`Scripts/`のソースは、Shift-JIS（CP932）・CRLFで書かれている。テストのビルドには`/source-charset:.932 /execution-charset:.932`を付けて、実行する環境の言語設定に関わらず、そのとおりに読んでいる。日本語を含むソースを編集するときは、文字コードを変えないこと。

## CI

`.github/workflows/ci.yml`（GitHub Actions）が、push・プルリクエスト・手動実行のたびに、`windows-2025`（Visual Studio 2026）でテストをDebugとReleaseの両方でビルドして実行する。

ゲーム本体はCIの対象外。FBX SDKはリポジトリに同梱できず、クラウドのランナーにも入れられないとライセンス本文から読めるため（ADR-0005）。ゲーム本体のビルドは、手元で確認する。
