#pragma once
//インクルード
#include <string>
#include "StageObjectBase.h"
#include "../Engine/Direct3D.h"

namespace
{
    std::string stageBlockName = "StageBlock";
}

using std::string;
//StageBlockを管理するクラス
class StageBlock : public StageObjectBase
{
    int hModel_;    //モデル番号
public:
    //コンストラクタ
    //引数:parent 親オブジェクト(SceneManager)
    StageBlock(GameObject* _parent);

    //デストラクタ
    ~StageBlock();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    int GetModelHandle() { return hModel_; }
};