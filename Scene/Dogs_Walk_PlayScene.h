#pragma once
//インクルード
#include <vector>
#include "../Engine/GameObject.h"
#include "../Engine/Camera.h"
#include "../ItemObject/ItemObjectManager.h"

namespace
{
    std::string soundDogs_Walk_PlaySceneNames[] =
    {
        "BGM",
        "LastBGM",
        "LastBGM2",
        "GameStop",
    };

    std::string Dogs_Walk_PlaySceneName = "Dogs_Walk_PlayScene";
}

class SceneManager;
class AttackPlayer;
class CollectPlayer;
class AIPlayer;
class ItemObjectManager;
class StageObjectManager;
class ImageManager;

/// <summary>
/// 骨一つ型プレイシーン
/// </summary>
class Dogs_Walk_PlayScene : public GameObject
{
public:
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
private:

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
    XMFLOAT3 playerFirstPos_[(int)PLAYERNUMSTATE::PLAYERNUM];
    float length_;
    //▼サウンドに関するメンバ変数
    int hSound_[(int)SOUNDSTATE::MAX];
    int randomValue_;
    float soundVolume_;
    float soundVolumeHalf_;
    //▼骨に関するメンバ変数
    int boneCount_;         //今ある骨の数
    XMFLOAT3 collectPlayerPosition_;
    XMVECTOR collectPlayerDirection_;
    float boneFrontPosition_;
    //▼木箱に関するメンバ変数
    int woodBoxCount_;      //今ある木箱の数
    XMFLOAT3 attackPlayerPosition_;         //アタックプレイヤーの位置
    XMVECTOR attackPlayerDirection_;   //アタックプレイヤーの向いてる位置
    float woodBoxFrontPosition_;              //前にどのくらい移動させるか
    //▼演出で使うメンバ変数
    float time_;                         //時間を表す
    bool isGameStop_;                  //ゲームが止まったか

    XMFLOAT3 Init = { 0.0f,0.0f,0.0f };
    XMVECTOR vPos[2] = {};
    XMFLOAT3 mouse = Init;
    XMFLOAT3 controller[2] = {};
    XMFLOAT3 rDir = { 0.0f,0.0f,1.0f };
    XMVECTOR Dir[2] = {};
    float sigmaRotY[2] = {};
    float sigmaRotX[2] = {};
    XMMATRIX mxRotX[2] = {};
    XMMATRIX mxRotY[2] = {};
    XMMATRIX rot[2] = {};
    XMFLOAT3 playerPos[2] = {};
    XMFLOAT3 floatDir[2] = {};

    const float mouseSens = 400;
    const float controllerSens = 50;

    SceneManager* pSceneManager_;
    AttackPlayer* pAttackPlayer_;
    CollectPlayer* pCollectPlayer_;
    AIPlayer* pAIPlayer_;
    ItemObjectManager* pItemObjectManager_;
    StageObjectManager* pStageObjectManager_;
    ImageManager* pImageManager_;
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="pParent">親オブジェクト</param>
    Dogs_Walk_PlayScene(GameObject* _pParent);
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
    XMFLOAT3 GetCollectPlayerPosition() { return collectPlayerPosition_; }
    void AddBoneCount(int _boneCount) { boneCount_ += _boneCount; }
    void AddWoodBoxCount(int _woodBoxCount) { woodBoxCount_ += _woodBoxCount; }
    ItemObjectManager* GetItemObjectManager() { return pItemObjectManager_; }
    void HideCursor() { while (::ShowCursor(false) >= 0); }
    void SetGameStop();
};

