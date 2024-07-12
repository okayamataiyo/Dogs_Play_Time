#pragma once
//インクルード
#include "../Engine/GameObject.h"
#include "../Engine/Camera.h"
#include "../ItemObject/ItemObjectManager.h"

namespace
{
    std::string soundDogs_Fight_PlaySceneNames[] =
    {
        "BGM",
        "LastBGM",
        "LastBGM2",
        "GameStop",
    };

    std::string Dogs_Fight_PlaySceneName = "Dogs_Fight_PlayScene";
}

class SceneManager;
class AttackPlayer;
class CollectPlayer;
class ItemObjectManager;
class StageObjectManager;
class ImageManager;

/// <summary>
/// 骨一個型プレイシーン
/// </summary>
class Dogs_Fight_PlayScene : public GameObject
{
private:

    enum class SOUNDSTATE
    {
        BGM = 0,
        LASTBGM,
        LASTBGM2,
        GAMESTOP,
        MAX,
    };

    enum class PADIDSTATE
    {
        FIRST = 0,
        SECONDS,
        THIRD,
        FOUR,
    };

    int attackOrCollect_;
    int attackOrCollectInverse_;
    int stageBlockNum_;             //ステージブロックの数
    int lengthRecedes_;
    float degreesMin_;
    float degreesMax_;
    float degreesToRadians_;
    float vecLengthRecedes_;
    float vecLengthApproach_;
    int woodBoxCountMax_;
    int changeScore_;
    XMFLOAT3 camVec_[2];
    Transform floorPosition_[3];
    float length_;
    //▼サウンドに関するメンバ変数
    int hSound_[(int)SOUNDSTATE::MAX];
    int random_value_;
    float soundVolume_;
    float soundVolumeHalf_;
    //▼骨に関するメンバ変数
    int boneCount_;         //今ある骨の数
    //▼木箱に関するメンバ変数
    int woodBoxCount_;      //今ある木箱の数
    XMFLOAT3 attackPlayerPosition_;         //アタックプレイヤーの位置
    XMVECTOR attackPlayerDirection_;   //アタックプレイヤーの向いてる位置
    float woodBoxFrontPosition_;              //前にどのくらい移動させるか
    //▼演出で使うメンバ変数
    int time_;                         //時間を表す
    bool isGameStop_;                  //ゲームが止まったか
    bool isPause_;

    SceneManager* pSceneManager_;
    AttackPlayer* pAttackPlayer_;
    CollectPlayer* pCollectPlayer_;
    ItemObjectManager* pItemObjectManager_;
    StageObjectManager* pStageObjectManager_;
    ImageManager* pAttackImageManager_;
    ImageManager* pCollectImageManager_;

public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="pParent">親オブジェクト</param>
    Dogs_Fight_PlayScene(GameObject* _pParent);
    void Initialize() override;
    void Update() override;
    void BothViewDraw() override;
    void LeftViewDraw() override;
    void RightViewDraw() override;
    void UPSubViewDraw() override;
    void Release() override;
    void BoneSummons();
    std::vector<int> GetWoodBoxs() { return pItemObjectManager_->GetWoodBoxs(); }
    XMFLOAT3 GetAttackPlayerPosition() { return attackPlayerPosition_; }
    void AddBoneCount(int _boneCount) { boneCount_ += _boneCount; }
    void AddWoodBoxCount(int _woodBoxCount) { woodBoxCount_ += _woodBoxCount; }
    ItemObjectManager* GetItemObjectManager() { return pItemObjectManager_; }
    void HideCursor() { while (::ShowCursor(false) >= 0); }
    void SetGameStop();
};

