#pragma once
//�C���N���[�h
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
/// ����^�v���C�V�[��
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
    int stageBlockNum_;             //�X�e�[�W�u���b�N�̐�
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
    //���T�E���h�Ɋւ��郁���o�ϐ�
    int hSound_[(int)SOUNDSTATE::MAX];
    int randomValue_;
    float soundVolume_;
    float soundVolumeHalf_;
    //�����Ɋւ��郁���o�ϐ�
    int boneCount_;         //�����鍜�̐�
    XMFLOAT3 collectPlayerPosition_;
    XMVECTOR collectPlayerDirection_;
    float boneFrontPosition_;
    //���ؔ��Ɋւ��郁���o�ϐ�
    int woodBoxCount_;      //������ؔ��̐�
    XMFLOAT3 attackPlayerPosition_;         //�A�^�b�N�v���C���[�̈ʒu
    XMVECTOR attackPlayerDirection_;   //�A�^�b�N�v���C���[�̌����Ă�ʒu
    float woodBoxFrontPosition_;              //�O�ɂǂ̂��炢�ړ������邩
    //�����o�Ŏg�������o�ϐ�
    float time_;                         //���Ԃ�\��
    bool isGameStop_;                  //�Q�[�����~�܂�����

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
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="pParent">�e�I�u�W�F�N�g</param>
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

