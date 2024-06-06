#pragma once
//インクルード
#include "../Engine/GameObject.h"

namespace
{
	enum class PLAYERSTATE {
		COLLECTPLAYER = 0,
		ATTACKPLAYER,
		PLAYERNUM
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
	//▼ゲーム演出に関するメンバ変数
	struct GameData
	{
		int FPS_;
		int timeCounter_;
		int timeLimit_;
		float fallLimit_;
		int score_;			//得点
		int scoreAmount_;	//得点の量
		int scoreMax_;		//最大得点量
		int scoreTimeCounter_;	//得点を加算する時間
		int scoreTimeCounterWait_;	//得点を加算する待機時間
		int padID_;
		int walkOrFight_;
		GameData()
			:FPS_{ 600 }, timeCounter_{ 0 }, timeLimit_{ 60 }, fallLimit_{ 100.0f }, score_{ 0 }, scoreAmount_{ 10 }, scoreMax_{ 150 },scoreTimeCounter_{0},scoreTimeCounterWait_{30}, padID_{0}, walkOrFight_{false}
		{

		}
	}gameData_;
	//▼サウンドに関するメンバ変数
	struct SoundData
	{
		float soundVolume_;
		float soundVolumeHalf_;
		SoundData()
			:soundVolume_{ 0.5f }, soundVolumeHalf_{ soundVolume_ / 2 }
		{

		}
	}soundData_;
	//▼プレイヤー移動に関するメンバ変数
	struct MoveData
	{
		XMVECTOR CamPositionVec_;
		XMFLOAT3 positionPrev_;		//1フレーム前の位置座標
		XMFLOAT3 padMoveSpeed_;
		bool isRun_;				//ダッシュしているかどうか
		bool isMove_;
		float isFling_;				//地面から離れているか
		MoveData()
			:CamPositionVec_{}, positionPrev_{ 0.0f,0.0f,0.0f }, padMoveSpeed_{ 0.3f,0.0f,0.3f }, isRun_{ false }, isMove_{ false }, isFling_{ 1.0f }
		{

		}
	}moveData_;
	//▼向き変えに関するメンバ変数
	struct DirData
	{
		XMVECTOR vecMove_;
		XMVECTOR vecCross_;
		XMVECTOR vecDirection_;
		float angle_;
		XMFLOAT3 vecCam_;
		DirData()
			:vecMove_{}, vecCross_{}, vecDirection_{}, angle_{ 0.0f }, vecCam_{ 0.0f,5.0f,-10.0f }
		{

		}
	}dirData_;
	//▼ジャンプに関するメンバ変数
	struct JumpData
	{
		float gravity_;				//重力
		float playerInitPosY_;
		float positionY_;			//プレイヤーのY座標に代入する値
		float positionTempY_;		//y座標をPrevに保存する
		float positionPrevY_;		//1時的にy座標を保存しておく
		float jumpPower_;			//ジャンプの時の力
		bool  isJump_;				//ジャンプしてるかしていないか
		JumpData()
			:gravity_{ 0.007f }, playerInitPosY_{0.6f}, positionY_{}, positionTempY_{ 0.0f }, positionPrevY_{ 0.0f }, jumpPower_{ 0.3f }, isJump_{ false }
		{

		}
	}jumpData_;
	//▼飛びつきに関するメンバ変数
	struct DiveData
	{
		float divePower_;			//飛びつきの時の力
		float diveSpeed_;
		bool isDive_;
		bool isDived_;
		int diveTime_;
		int diveDuration_;	//飛びつきが継続する時間
		int diveTimeWait_;	//飛びつきが終わるまでの時間
		DiveData()
			:divePower_{ 0.1f }, diveSpeed_{ 0.6f }, isDive_{ false }, isDived_{ false }, diveTime_{ 0 }, diveDuration_{ 1 }, diveTimeWait_{ 30 }
		{

		}
	}diveData_;
	//▼すり抜け床に関するメンバ変数
	struct FloorData
	{
		bool isOnFloor_;		//すり抜け床にレイを飛ばしているかどうか
		FloorData()
			:isOnFloor_{ false }
		{

		}
	}floorData_;
	//▼木箱に関するメンバ変数
	struct WoodBoxData
	{
		const int woodBoxDeath_ = 1;	//木箱が無くなった時に木箱カウントを減らす変数
		std::string woodBoxNumber_;
		float dotProduct_;
		float angleDegrees_;
		float angleDegreesMax_;	//ジャンプになるか壁になるかの境目の変数
		WoodBoxData()
			:woodBoxNumber_{ "" }, dotProduct_{ 0.0f }, angleDegrees_{ 0.0f }, angleDegreesMax_{ 80.0f }
		{

		}
	}woodBoxData_;
	//▼スタンに関するメンバ変数
	struct StunData
	{
		int stunTimeCounter_;	//スタンしてからどのくらい経ったか
		int stunLimit_;		//スタンしている時間
		int hitStopTime_;		//ヒットストップの時間
		int getUpTime_;		//スタンしてから起き上がる時間
		float knockbackSpeed_;
		XMVECTOR vecKnockbackDirection_;
		bool isStun_;		//スタンしているかどうか
		bool isKnockBack_;	//ノックバックしているかどうか
		StunData()
			:stunTimeCounter_{ 0 }, stunLimit_{ 0 }, hitStopTime_{ 10 }, getUpTime_{ 30 }, knockbackSpeed_{ 0.3f }, vecKnockbackDirection_{}, isStun_{ false }, isKnockBack_{ false }
		{

		}
	}stunData_;
	//▼慣性に関するメンバ変数

	//▼壁判定に関するメンバ変数
	struct WallData
	{
		float distMax_;					//衝突点までの最大距離
		float inTheWall_;				//壁に埋まっているか
		float rayFloorDistDown_;
		float rayStageDistDown_;
		WallData()
			:distMax_{ 99999.0f }, inTheWall_{ 1.5f }, rayFloorDistDown_{ 0.0f }, rayStageDistDown_{ 0.0f }
		{

		}
	}wallData_;
	//▼骨に関するメンバ変数
	struct BoneData
	{
		int decBoneCount_;
		bool isBoneTatch_;
		int killTime_;
		int killTimeWait_;
		int killTimeMax_;
		BoneData()
			:decBoneCount_{ -1 }, isBoneTatch_{ false }, killTime_{ 99999 }, killTimeWait_{ 30 }, killTimeMax_{ 99999 }
		{

		}
	}boneData_;
	//▼アニメーションに関するメンバ変数
	struct AnimData
	{
		int startFrame_;
		int endFrame_;
		float animSpeed_;
		AnimData()
			:startFrame_{ 0 }, endFrame_{ 0 }, animSpeed_{ 0.0f }
		{

		}
	}animData_;
protected:
	PlayerBase(GameObject* _parent, std::string _name);
	//初期化
	void Initialize()   override;

	//更新
	void Update()       override;

	//描画
	void BothViewDraw() override;
	void LeftViewDraw() override;
	void RightViewDraw() override;

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

	virtual void PlayerScore();

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
	
	void ApplyMovement(float moveX, float moveZ);

	virtual void IsMove();

	virtual void IsJump();

	virtual void IsRun();

	virtual void IsStun();

	virtual void IsDive();

	virtual void SetVecPos(XMVECTOR _vecMove) = 0;

	virtual void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f);

	virtual bool GetIsMove() = 0;

	virtual bool GetIsJump() = 0;

	virtual bool GetIsRun() = 0;

	virtual bool GetIsStun() = 0;

	virtual bool GetIsDive() = 0;

	virtual XMVECTOR GetVecPos() = 0;

	virtual int GetPadID() = 0;

	virtual int GetModelHandle() = 0;
};

