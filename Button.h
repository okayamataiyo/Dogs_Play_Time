#pragma once
//�C���N���[�h
#include "Engine/Model.h"
#include "Engine/GameObject.h"

class Button : public GameObject
{
private:
	int hModel_;
	int hPushModel_;
	int pushed_;
	XMFLOAT3 startPos_;
	XMFLOAT3 endPos_;
	XMFLOAT3 center_;	//�{�^���̒��S���W(��ʍ��W)
	XMFLOAT3 size_;		//�{�^���̉摜�T�C�Y
	float totalTime_;
	float currentTime_;
public:
	Button(GameObject* _pParent);
	~Button();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void SetPosition(int _x, int _y);						//�{�^���\���ʒu
	void Push(bool _pushed) { this->pushed_ = _pushed; }	//�{�^�������������A�����ĂȂ���
	bool MouseInArea(XMFLOAT3 _mousePos);					//�}�E�X���{�^�����ɓ����Ă��邩
	bool IsMoving() { return(currentTime_ < totalTime_); }	//�����Ă���Ȃ�true
	void ForceMoveEnd();									//�����I�Ɉړ����I��������
};

