#pragma once
//インクルード
#include "../Engine/GameObject.h"

class  PlayerBase : public GameObject
{
protected:

	enum class PLAYERSTATE
	{
		WAIT = 0,
		WALK,
		RUN,
		JUMP,
		STUN,
	};

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
	float controllerMoveSpeed_;
	float mouseMoveSpeed_;
	bool isDash_;				//ダッシュしているかどうか
	float isFling_;				//地面から離れているか
	float deadZone_;			//コントローラーのデットゾーン
	float pi_;					//円周率
	float halfPi_;				//円周率の半分
	float dashSpeed_;
	float walkSpeed_;
	//▼向き変えに関するメンバ変数
	XMVECTOR vecMove_;
	XMVECTOR vecLength_;
	XMVECTOR vecDot_;
	XMVECTOR vecCross_;
	float length_;
	float dot_;
	float angle_;
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
	bool isStun_;		//スタンしているかどうか
	bool isKnockBack_;	//ノックバックしているかどうか
	//▼慣性に関するメンバ変数

	//▼壁判定に関するメンバ変数
	float distMax_;					//衝突点までの最大距離
	float inTheWall_;				//壁に埋まっているか
	float outerWallPosFront_;		//前の外壁の位置
	float outerWallPosBack_;		//後ろの外壁の位置
	float outerWallPosLeft_;		//左の外壁の位置
	float outerWallPosRight_;		//右の外壁の位置
	float rayFloorDistUp_;			//上のすり抜け床とプレイヤーの差距離
	float rayFloorDistDown_;		//下のすり抜け床とプレイヤーの差距離
	float rayStageBlockDistDown_;
	float rayStageDistDown_;		//地面とプレイヤーの差距離
	float rayStageDistFront_;		//前壁とプレイヤーの差距離
	float rayStageDistBack_;		//後壁とプレイヤーの差距離
	float rayStageDistLeft_;		//左壁とプレイヤーの差距離
	float rayStageDistRight_;		//右壁とプレイヤーの差距離
protected:
	PlayerBase(GameObject* _parent, std::string _name)
		:GameObject(_parent, _name) {}
	//初期化
	void Initialize()   override {};

	//更新
	void Update()       override {};

	//描画
	void Draw()         override {};

	//開放
	void Release()      override {};

	virtual void UpdateReady() = 0;

	virtual void UpdatePlay() = 0;

	virtual void UpdateGameOver() = 0;

	virtual void PlayerFall() = 0;

	virtual void PlayerMove() = 0;

	virtual void PlayerJump() = 0;

	virtual void PlayerJumpPower() = 0;

	virtual void PlayerDive() = 0;

	virtual void PlayerDivePower() = 0;

	virtual void PlayerKnockback() = 0;

	virtual void PlayerRayCast() = 0;

	virtual void Stun(int _timeLimit = 60) = 0;

	virtual void SetVecPos(XMVECTOR _vecMove) = 0;

	virtual void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f) = 0;

	virtual XMVECTOR GetVecPos() = 0;

	virtual PLAYERSTATE GetPlayerState() = 0;

	virtual int GetPadID() = 0;

	virtual bool IsMoving() = 0;

	virtual int GetModelHandle() = 0;
};

