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

## 実装時の決定
区切りごとに追記する（①オブジェクトの仕組み、②Engineの資源、③COMと後始末）。

### 所有のルール（全体）
- 所有は`std::unique_ptr`で持つ（共有するものは`std::shared_ptr`、COMは`ComPtr`）。生ポインターは「借りているだけ」で、持ち主より長く持たない。
- 所有を渡す関数は、`std::unique_ptr`を受け取る（呼び出し側が`std::make_unique`で作る）。借りるだけの関数は、生ポインターを受け取る。
- 解放の順序が大事なものは、`Release()`などで明示的に`reset`して、デストラクターに任せない（例：プレイヤーの`StateManager`。`ExitState`がプレイヤーのほかのメンバーを使うため）。
- 子の一覧は`std::list`のままにする（更新中に`Instantiate`で子が増えても、反復が壊れないため）。

### 区切り①（オブジェクトの仕組み）
- `StateManager`: 状態を`unique_ptr`で持つ。`AddState`は`unique_ptr`を受け取り、同じ名前があるときは、渡されたものを破棄してすでにあるものを返す（今までは渡したものが漏れていた）。デストラクターが、名前の順に`ExitState`を呼んでから消す順序は同じ。プレイヤー3種の`StateManager`は`unique_ptr`で持ち、`Release()`で消す。
- コライダー: `GameObject`が`unique_ptr`で持つ。`AddCollider`は`unique_ptr`を受け取り、追加したコライダーを返す。各クラスの`pCollision_`は借りているだけ。
- `GameObject`の子: 親が`unique_ptr`で持つ。`Instantiate` / `InstantiateFront`は、親が必須になった（今までは親なしで呼ぶと、誰も持たなかった。呼び出し側に該当はなかった）。子を親に渡してから`Initialize()`を呼ぶ順序は同じ。`ReleaseSub()`と`KillAllChildren()`は、葉から`Release()`、デストラクターの順序のまま。`ReleaseSub()`を通さずに消した親は、子も消す（今までは子が漏れていた）。`GetChildList()`は、読むだけの型になった。`Main`のルートも`unique_ptr`で持ち、同じ位置で消す。
- Scriptsの持ち主: `StageObjectManager`と`ItemObjectManager`（シーンが持つ）、`UIManager`の`Text`、`AIPlayer`のビヘイビアツリーのノードを`unique_ptr`で持つ。この4種は、今まで解放されていなかった。プレイヤーなどが持っている`ItemObjectManager*`は、シーンから借りているだけ（プレイヤーはシーンより先に消える）。
- 順序は、`Tests/GameObjectTest.cpp`と`Tests/StateManagerTest.cpp`が、更新・描画・解放・破棄のたびに確かめている。

### 確かめたこと（区切り①）
- テストのDebug・Releaseのビルドと全件合格（58件・256の検証）。ゲーム本体のDebug・Releaseのビルド（警告は基線と同じ）と起動。
- シーンを自動で巡回して、落ちないこと（タイトル、ゲーム選択、犬の選択、散歩、ゲームオーバーを2周、10回の切替）。人の入力やコントローラーでシーンが勝手に切り替わらないよう、入力を読まない確認用のビルドで行った。
- 終了時のメモリリーク（CRTのリーク報告）が、61ブロック・4,344バイトから0ブロックになったこと。
- 必須のモデルが欠けたとき（`Floor`、`CollectPlayer`、`AttackPlayer`と`AIPlayer`）に、ダイアログを出して終了コード1で終わり、落ちないこと（ADR-0007の動きのまま）。

### 残り
- 区切り②（Engineの資源）と区切り③（COMと後始末）。
