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

## 後続の対応
- COM初期化は、ADR-0007で`Main`に移した。`Audio::Initialize()`はCOMを初期化せず、`Engine/Texture.cpp`の`Texture::Load`が毎回呼んでいた`CoInitialize(NULL)`もなくした。アプリケーション寿命で一度だけ初期化・解放するという本ADRの決定は変わらない。
- `Audio::Initialize()`が失敗したときは、ADR-0007の決定どおり、音声エンジンを必須として扱い、原因を示して起動を中止する。
