#pragma once
//インクルード
#include <string>
#include "../Engine/Direct3D.h"
#include "../Scene/PlayScene.h"
#include "ItemObjectBase.h"

namespace
{
    std::string frameBoxName = "FrameBox";
    std::string boxColliderName = "BoxCollider";
}

class AttackPlayer;

//FrameBoxを管理するクラス
class FrameBox : public ItemObjectBase
{
private:
    int hModel_;            //モデル
    bool isBreak_;          //木箱が壊されたか
    float frameBoxInitPosY_;
    std::vector<int> FrameBoxs_;
    PlayScene* pPlayScene_;
    AttackPlayer* pAttackPlayer_;
public:
    //コンストラクタ
    //引数:parent 親オブジェクト(SceneManager)
    FrameBox(GameObject* _pParent);

    //デストラクタ
    ~FrameBox();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    void Move();

    void OnCollision(GameObject* _pTarget);

    int GetModelHandle() { return hModel_; }

    XMVECTOR GetVecPos() { return XMLoadFloat3(&transform_.position_); }

    void SetFrameBoxBreak() { isBreak_ = true; }
};