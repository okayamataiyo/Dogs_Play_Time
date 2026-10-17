# ADR-0001: Windows x64を対象プラットフォームとする

- Status: Accepted
- Implementation: Done

## 背景
本プロジェクトはWin32 APIとDirect3D 11を使うWindows向けアプリケーションである。Visual StudioプロジェクトにはWin32とx64の構成がある一方、FBX SDKの参照設定はx64向けである。監査方針として、Windows x64のみをサポート対象とすることが合意された。

## 決定
サポート対象をWindows x64に限定し、既存のWin32/Direct3D 11基盤を維持する。クロスプラットフォーム対応は本プロジェクトの目標に含めない。

## 影響
- ビルド、テスト、CIの対象はWindows x64とする。
- Win32構成はサポート対象外として扱い、今後のプロジェクト設定整理で無効化または削除を検討する。
- Windows固有APIとDirect3D 11への依存は継続する。
