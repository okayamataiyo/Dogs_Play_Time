#pragma once
//�C���N���[�h
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

//FrameBox���Ǘ�����N���X
class FrameBox : public ItemObjectBase
{
private:
    int hModel_;            //���f��
    bool isBreak_;          //�ؔ����󂳂ꂽ��
    float frameBoxInitPosY_;
    std::vector<int> FrameBoxs_;
    PlayScene* pPlayScene_;
    AttackPlayer* pAttackPlayer_;
public:
    //�R���X�g���N�^
    //����:parent �e�I�u�W�F�N�g(SceneManager)
    FrameBox(GameObject* _pParent);

    //�f�X�g���N�^
    ~FrameBox();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    void Move();

    void OnCollision(GameObject* _pTarget);

    int GetModelHandle() { return hModel_; }

    XMVECTOR GetVecPos() { return XMLoadFloat3(&transform_.position_); }

    void SetFrameBoxBreak() { isBreak_ = true; }
};