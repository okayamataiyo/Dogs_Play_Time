#pragma once
//�C���N���[�h
#include <string>
#include "../Engine/Direct3D.h"
#include "ItemObjectBase.h"

namespace
{
    std::string floorName = "Floor";
}

//Floor���Ǘ�����N���X
class Floor : public ItemObjectBase
{
    enum class FLOORSTATE
    {
        UP = 0,
        DOWN,
        WAIT,
    };
    int hModel_;    //���f���ԍ�
    int floorState_;  //�オ���Ă邩�������Ă邩
    float upVelocity_;
    float downVelocity_;
    float positionUpMax_;
    float positionDownMax_;
public:
    //�R���X�g���N�^
    //����:parent �e�I�u�W�F�N�g(SceneManager)
    Floor(GameObject* _parent);

    //�f�X�g���N�^
    ~Floor();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    int GetModelHandle() override { return hModel_; }
};