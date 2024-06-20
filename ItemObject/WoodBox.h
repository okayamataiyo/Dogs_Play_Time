#pragma once
//�C���N���[�h
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

//WoodBox���Ǘ�����N���X
class WoodBox : public ItemObjectBase
{
private:
    int hModel_;            //���f��
    int hSound_;            //�T�E���h�f�[�^
    float soundVolume_;
    bool isBreak_;            //�ؔ����󂳂ꂽ��
    int walkOrFight_;
    std::vector<int> woodBoxs_;

    //�������Ɋւ��郁���o�ϐ�
    float gravity_;
    float woodBoxInitposY_;
    float positionY_;            //�ؔ���Y���W�ɑ������l
    float positionPrevY_;        //1�t���[���O��Y���W
    float positionTempY_;        //1���I�ɍ��W��ۑ�
    bool isJump_;           //���ł��邩
    //�����C�L���X�g�Ɋւ��郁���o�ϐ�
    bool  isOnWoodBox_;      //�ؔ��̏�ɂ��邩
    float rayWoodBoxDist_;  //�ؔ��̃f�B�X�g
    float rayStageDistDown_;    //�X�e�[�W�̃f�B�X�g
    float isFling_;



    GameObject* pParent_;
    Dogs_Walk_PlayScene* pDogs_Walk_PlayScene_;
    Dogs_Fight_PlayScene* pDogs_Fight_PlayScene_;
    AttackPlayer* pAttackPlayer_;
    SphereCollider* pCollision_;

public:
    //�R���X�g���N�^
    //����:parent �e�I�u�W�F�N�g(SceneManager)
    WoodBox(GameObject* _pParent);

    //�f�X�g���N�^
    ~WoodBox();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void BothViewDraw() override;
    void LeftViewDraw() override;
    void RightViewDraw() override;
    void UPSubViewDraw() override;

    //�J��
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