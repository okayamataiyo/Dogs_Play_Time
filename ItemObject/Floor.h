#pragma once
//インクルード
#include <string>
#include "../Engine/Direct3D.h"
#include "ItemObjectBase.h"

namespace
{
    std::string floorName = "Floor";
}

//Floorを管理するクラス
class Floor : public ItemObjectBase
{
    enum class FLOORSTATE
    {
        UP = 0,
        DOWN,
        WAIT,
    };
    int hModel_;    //モデル番号
    int floorState_;  //上がってるか下がってるか
    float upVelocity_;
    float downVelocity_;
    float positionUpMax_;
    float positionDownMax_;
public:
    //コンストラクタ
    //引数:parent 親オブジェクト(SceneManager)
    Floor(GameObject* _parent);

    //デストラクタ
    ~Floor();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    int GetModelHandle() override { return hModel_; }
};