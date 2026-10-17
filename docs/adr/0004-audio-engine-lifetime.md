# ADR-0004: Audioエンジンとシーン音声データの寿命を分ける

- Status: Accepted
- Implementation: Done

## 背景
`Engine/Main.cpp`は起動時に`Audio::Initialize()`を呼ぶ。さらに`Engine/SceneManager.cpp`はシーン切替時に`Audio::Release()`の後で再度`Audio::Initialize()`を呼ぶ。`Engine/Audio.cpp`の`Release()`はシーン音声データを解放するが、XAudio2エンジンとマスターボイスの解放はアプリ終了時の`AllRelease()`で行われる。

## 決定
XAudio2エンジンとCOM初期化はアプリケーション寿命とし、起動時に一度だけ初期化して終了時に一度だけ解放する。音声ファイルやソースボイスなどシーンに属するデータは、シーン単位でロード・解放する。

## 影響
- エンジン初期化/終了とシーン音声データのロード/解放を別の責務として扱う。
- シーン切替時にエンジンやCOMを再初期化しない。
- 部分初期化に失敗した場合の解放順序を明確にする。

## 残課題
- `Engine/Texture.cpp`の`Texture::Load`は読み込みのたびに`CoInitialize(NULL)`を呼んでいる。COM初期化は`Audio::Initialize()`が起動時に一度だけ行う方針のまま、この呼び出しの整理はADR-0007で扱う。
- `Audio::Initialize()`が失敗したときの挙動（音声なしで続行するか、起動を中止するか）と、`Engine/Main.cpp`での戻り値の扱いは、ADR-0007で決める。本ADRの範囲は、失敗時に作りかけのものを正しい順序で解放し、失敗を戻り値（HRESULT）で返すところまでとする。
