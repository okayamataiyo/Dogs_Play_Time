#pragma once
//インクルード
#include <string>
#include "../Engine/Direct3D.h"
#include "ItemObjectBase.h"

namespace
{
    std::string soundWoodBoxName = "WoodBoxBreak";
    std::string woodBoxName = "WoodBox";
}

class GameObject;
class AttackPlayer;
class SphereCollider;
class Dogs_Walk_PlayScene;
class Dogs_Fight_PlayScene;

//WoodBoxを管理するクラス
class WoodBox : public ItemObjectBase
{
private:
    int hModel_;            //モデル
    int hSound_;            //サウンドデータ
    float soundVolume_;
    bool isBreak_;            //木箱が壊されたか
    int walkOrFight_;
    std::vector<int> woodBoxs_;

    //▼落下に関するメンバ変数
    float gravity_;
    float woodBoxInitposY_;
    float positionY_;            //木箱のY座標に代入する値
    float positionPrevY_;        //1フレーム前のY座標
    float positionTempY_;        //1時的に座標を保存
    bool isJump_;           //飛んでいるか
    //▼レイキャストに関するメンバ変数
    bool  isOnWoodBox_;      //木箱の上にいるか
    float rayWoodBoxDist_;  //木箱のディスト
    float rayStageDistDown_;    //ステージのディスト
    float isFling_;



    GameObject* pParent_;
    Dogs_Walk_PlayScene* pDogs_Walk_PlayScene_;
    Dogs_Fight_PlayScene* pDogs_Fight_PlayScene_;
    AttackPlayer* pAttackPlayer_;
    SphereCollider* pCollision_;

public:
    //コンストラクタ
    //引数:parent 親オブジェクト(SceneManager)
    WoodBox(GameObject* _pParent);

    //デストラクタ
    ~WoodBox();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void BothViewDraw() override;
    void LeftViewDraw() override;
    void RightViewDraw() override;
    void UPSubViewDraw() override;

    //開放
    void Release() override;

    void WoodBoxFall();

    void WoodBoxMove();

    void WoodBoxRayCast();

    void WoodBoxDeath();

    void OnCollision(GameObject* _pTarget);

    int GetModelHandle() { return hModel_; }

    XMVECTOR GetVecPos() { return XMLoadFloat3(&transform_.position_); }

    void SetWoodBoxBreak() { isBreak_ = true; }
};