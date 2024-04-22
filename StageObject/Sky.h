#pragma once
//インクルード
#include "StageObjectBase.h"

namespace
{
    std::string skyName = "Sky";
}

//Skyを管理するクラス
class Sky : public StageObjectBase
{
    int hModel_;    //モデル番号
public:
    //コンストラクタ
    //引数:parent 親オブジェクト(SceneManager)
    Sky(GameObject* _parent);

    //デストラクタ
    ~Sky();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    //指定した位置が通れるか通れないかを調べる
    //引数:x,z  調べる位置
    //戻り値:通れない=true/通れない=false
    //bool IsWall(int x, int z);
    //▼セッター・ゲッター
    void SetRotate(XMFLOAT3 _rotate) { transform_.rotate_ = _rotate; }

    int GetModelHandle() { return hModel_; }
};