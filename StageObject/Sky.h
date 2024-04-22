#pragma once
//�C���N���[�h
#include "StageObjectBase.h"

namespace
{
    std::string skyName = "Sky";
}

//Sky���Ǘ�����N���X
class Sky : public StageObjectBase
{
    int hModel_;    //���f���ԍ�
public:
    //�R���X�g���N�^
    //����:parent �e�I�u�W�F�N�g(SceneManager)
    Sky(GameObject* _parent);

    //�f�X�g���N�^
    ~Sky();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    //�w�肵���ʒu���ʂ�邩�ʂ�Ȃ����𒲂ׂ�
    //����:x,z  ���ׂ�ʒu
    //�߂�l:�ʂ�Ȃ�=true/�ʂ�Ȃ�=false
    //bool IsWall(int x, int z);
    //���Z�b�^�[�E�Q�b�^�[
    void SetRotate(XMFLOAT3 _rotate) { transform_.rotate_ = _rotate; }

    int GetModelHandle() { return hModel_; }
};