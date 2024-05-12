#pragma once
//インクルード
#include "../Engine/GameObject.h"

namespace
{
	enum Player {
		collectPlayer = 0,
		attackPlayer,
		playerNum
	};
}

class  PlayerBase : public GameObject
{
protected:
	enum class GAMESTATE
	{
		READY = 0,
		PLAY,
		GAMEOVER,
	};
	//▼UIに関するメンバ変数
	int drawScoreTextX_;
	int drawScoreTextY_;
	int drawScoreNumberX_;
	int drawScoreNumberY_;
	//▼ゲーム演出に関するメンバ変数
	int FPS_;
	int timeCounter_;
	int timeLimit_;
	float fallLimit_;
	int score_;			//得点
	int scoreAmount_;	//得点の量
	int scoreMax_;		//最大得点量
	int padID_;
	//▼サウンドに関するメンバ変数
	float soundVolume_;
	float soundVolumeHalf_;
	//▼プレイヤー移動に関するメンバ変数
	XMVECTOR CamPositionVec_;
	XMFLOAT3 positionPrev_;		//1フレーム前の位置座標
	XMFLOAT3 controllerMoveSpeed_;
	bool isRun_;				//ダッシュしているかどうか
	bool isMove_;
	float isFling_;				//地面から離れているか
	//▼向き変えに関するメンバ変数
	XMVECTOR vecMove_;
	XMVECTOR vecCross_;
	XMVECTOR vecDirection_;
	float angle_;
	XMFLOAT3 floDir_;
	float floCameraLen_;
	float floKnockbackLenRecedes_;
	//▼ジャンプに関するメンバ変数
	float gravity_;				//重力
	float playerInitPosY_;
	float positionY_;			//プレイヤーのY座標に代入する値
	float positionTempY_;		//y座標をPrevに保存する
	float positionPrevY_;		//1時的にy座標を保存しておく
	float jumpPower_;			//ジャンプの時の力
	bool  isJump_;				//ジャンプしてるかしていないか
	//▼飛びつきに関するメンバ変数
	float divePower_;			//飛びつきの時の力
	float diveSpeed_;
	bool isDive_;
	bool isDived_;
	int diveTime_;
	int diveDuration_;	//飛びつきが継続する時間
	int diveTimeWait_;	//飛びつきが終わるまでの時間
	//▼すり抜け床に関するメンバ変数
	bool isOnFloor_;		//すり抜け床にレイを飛ばしているかどうか
	//▼木箱に関するメンバ変数
	const int woodBoxDeath_ = 1;	//木箱が無くなった時に木箱カウントを減らす変数
	std::string woodBoxNumber_;
	float dotProduct_;
	float angleDegrees_;
	float angleDegreesMax_;	//ジャンプになるか壁になるかの境目の変数
	//▼スタンに関するメンバ変数
	int stunTimeCounter_;	//スタンしてからどのくらい経ったか
	int stunLimit_;		//スタンしている時間
	int hitStopTime_;		//ヒットストップの時間
	int getUpTime_;		//スタンしてから起き上がる時間
	float knockbackSpeed_;
	XMVECTOR vecKnockbackDirection_;
	bool isStun_;		//スタンしているかどうか
	bool isKnockBack_;	//ノックバックしているかどうか
	//▼慣性に関するメンバ変数

	//▼壁判定に関するメンバ変数
	float distMax_;					//衝突点までの最大距離
	float inTheWall_;				//壁に埋まっているか
	float rayFloorDistDown_;
	float rayStageDistDown_;
	//▼アニメーションに関するメンバ変数
	int startFrame_;
	int endFrame_;
	float animSpeed_;
protected:
	PlayerBase(GameObject* _parent, std::string _name);
	//初期化
	void Initialize()   override;

	//更新
	void Update()       override;

	//描画
	void Draw()         override;

	//開放
	void Release()      override;

	virtual void UpdateReady() = 0;

	virtual void UpdatePlay() = 0;

	virtual void UpdateGameOver() = 0;

	virtual void PlayerWaitStateFunc();
	virtual void PlayerWalkStateFunc();
	virtual void PlayerRunStateFunc();
	virtual void PlayerJumpStateFunc();
	virtual void PlayerStunStateFunc();

	virtual void PlayerCamera();

	virtual void PlayerFall() = 0;

	virtual void PlayerMove() = 0;

	virtual void PlayerJump() = 0;

	virtual void PlayerJumpPower() = 0;

	virtual void PlayerDive() = 0;

	virtual void PlayerDivePower() = 0;

	virtual void PlayerKnockback() = 0;

	virtual void PlayerRayCast() = 0;

	virtual void PlayerRevival();

	virtual void PlayerStun(int _timeLimit = 60) = 0;

	virtual void IsMove();

	virtual void IsJump();

	virtual void IsRun();

	virtual void IsStun();

	virtual void IsDive();

	virtual void SetVecPos(XMVECTOR _vecMove) = 0;

	virtual void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f) = 0;

	virtual bool GetIsMove() = 0;

	virtual bool GetIsJump() = 0;

	virtual bool GetIsRun() = 0;

	virtual bool GetIsStun() = 0;

	virtual bool GetIsDive() = 0;

	virtual XMVECTOR GetVecPos() = 0;

	virtual int GetPadID() = 0;

	virtual int GetModelHandle() = 0;
};

