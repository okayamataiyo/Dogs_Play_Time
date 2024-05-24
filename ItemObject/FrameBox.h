#pragma once
//インクルード
#include <string>
#include "../Engine/Direct3D.h"
#include "ItemObjectBase.h"

namespace
{
    std::string frameBoxName = "FrameBox";
    std::string boxColliderName = "BoxCollider";
}

class AttackPlayer;
class Dogs_Walk_PlayScene;
class Dogs_Fight_PlayScene;

//FrameBoxを管理するクラス
class FrameBox : public ItemObjectBase
{
private:
    int hModel_;            //モデル
    bool isBreak_;          //木箱が壊されたか
    float frameBoxInitPosY_;
    int walkOrFight_;
    std::vector<int> FrameBoxs_;
    Dogs_Walk_PlayScene* pDogs_Walk_PlayScene_;
    Dogs_Fight_PlayScene* pDogs_Fight_PlayScene_;
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