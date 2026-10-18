# ADR-0003: 所有権を段階的にRAIIへ移行する

- Status: Accepted
- Implementation: In progress

## 背景
`Engine/GameObject.h`では子オブジェクトを`std::list<GameObject*>`で保持し、`Instantiate`が`new`で生成する。解放は`Engine/GameObject.cpp`の再帰的な`Release`/`delete`処理に依存する。`Scripts/State/StateManager.h`も状態オブジェクトをraw pointerで保持している。

## 決定
GameObjectの親子階層や既存の振る舞いを維持しつつ、所有するポインターを段階的にRAII管理へ移行する。参照だけを表す親ポインターなどは非所有参照として区別する。オブジェクトシステム全体の一括置換は行わない。

## 影響
- 所有者と非所有参照の責務を明確にする。
- 変更は境界ごとに進め、シーン破棄・状態切替の順序をテストで保護する。
- 既存の`Release`処理との責務重複を、移行時に整理する。
