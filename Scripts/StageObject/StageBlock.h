#pragma once
//�C���N���[�h
#include <string>
#include "StageObjectBase.h"
#include "../../Engine/Direct3D.h"

namespace
{
    std::string stageBlockName = "StageBlock";
}

using std::string;
//StageBlock���Ǘ�����N���X
class StageBlock : public StageObjectBase
{
    int hModel_;    //���f���ԍ�
public:
    //�R���X�g���N�^
    //����:parent �e�I�u�W�F�N�g(SceneManager)
    StageBlock(GameObject* _parent);

    //�f�X�g���N�^
    ~StageBlock();

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

    int GetModelHandle() { return hModel_; }
};