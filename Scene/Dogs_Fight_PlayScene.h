#pragma once
//�C���N���[�h
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
/// ����^�v���C�V�[��
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
    float length_;
    //���T�E���h�Ɋւ��郁���o�ϐ�
    int hSound_[(int)SOUNDSTATE::MAX];
    int random_value_;
    float soundVolume_;
    float soundVolumeHalf_;
    //�����Ɋւ��郁���o�ϐ�
    int boneCount_;         //�����鍜�̐�
    //���ؔ��Ɋւ��郁���o�ϐ�
    int woodBoxCount_;      //������ؔ��̐�
    XMFLOAT3 attackPlayerPosition_;         //�A�^�b�N�v���C���[�̈ʒu
    XMVECTOR attackPlayerDirection_;   //�A�^�b�N�v���C���[�̌����Ă�ʒu
    float woodBoxFrontPosition_;              //�O�ɂǂ̂��炢�ړ������邩
    //�����o�Ŏg�������o�ϐ�
    int time_;                         //���Ԃ�\��
    bool isGameStop_;                  //�Q�[�����~�܂�����
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
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="pParent">�e�I�u�W�F�N�g</param>
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

