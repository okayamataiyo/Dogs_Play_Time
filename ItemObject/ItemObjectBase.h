#pragma once
//�C���N���[�h
#include "../Engine/GameObject.h"
class ItemObjectBase : public GameObject
{
public:
    ItemObjectBase(GameObject* _parent,std::string _name)
        :GameObject(_parent,_name){}
    //������
    void Initialize()   override {};

    //�X�V
    void Update()       override {};

    //�`��
    void Draw()         override {};

    //�J��
    void Release()      override {};

    virtual int GetModelHandle() = 0;
};