#pragma once
#include "GameObject.h"

//-----------------------------------------------------------
//�J����
//-----------------------------------------------------------
namespace Camera
{
	//�������i�v���W�F�N�V�����s��쐬�j
	void Initialize();

	//�X�V�i�r���[�s��쐬�j
	void Update(int _type);

	//���_�i�J�����̈ʒu�j��ݒ�
	void SetPosition(XMFLOAT3 _position, int _type);

	//�œ_�i����ʒu�j��ݒ�
	void SetTarget(XMFLOAT3 _target, int _type);

	void SetIsChangeView(int _isChangeView);

	//�ʒu���擾
	XMFLOAT3 GetPosition(int _type);

	XMVECTOR VecGetPosition(int _type);

	//�œ_���擾
	XMFLOAT3 GetTarget(int _type);

	XMVECTOR VecGetTarget(int _type);

	//�r���[�s����擾
	XMMATRIX GetViewMatrix();

	//�v���W�F�N�V�����s����擾
	XMMATRIX GetProjectionMatrix();

	//�r���{�[�h�p��]�s����擾
	XMMATRIX GetBillboardMatrix();
};