#pragma once
//インクルード
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
	XMFLOAT3 center_;	//ボタンの中心座標(画面座標)
	XMFLOAT3 size_;		//ボタンの画像サイズ
	float totalTime_;
	float currentTime_;
public:
	Button(GameObject* _pParent);
	~Button();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void SetPosition(int _x, int _y);						//ボタン表示位置
	void Push(bool _pushed) { this->pushed_ = _pushed; }	//ボタンを押したか、押してないか
	bool MouseInArea(XMFLOAT3 _mousePos);					//マウスがボタン内に入っているか
	bool IsMoving() { return(currentTime_ < totalTime_); }	//動いているならtrue
	void ForceMoveEnd();									//強制的に移動を終了させる
};

