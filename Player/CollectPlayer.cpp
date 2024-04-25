//インクルード
#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"
#include "../Engine/Model.h"
#include "../Engine/Direct3D.h"
#include "../Engine/ImGui/imgui.h"
#include "../Engine/Text.h"
#include "../Engine/Audio.h"
#include "../Engine/VFX.h"
#include "../Engine/Global.h"
#include "../ItemObject/Floor.h"
#include "../ItemObject/WoodBox.h"
#include "../ItemObject/Bone.h"
#include "../ItemObject/ItemObjectManager.h"
#include "../StageObject/Stage.h"
#include "../StageObject/StageBlock.h"
#include "../State/StateManager.h"
#include "../State/PlayerState.h"
#include "CollectPlayer.h"
#include "AttackPlayer.h"

CollectPlayer::CollectPlayer(GameObject* _pParent)
    :PlayerBase(_pParent, collectPlayerName), hModel_{ -1 }, hSound_{ -1,-1,-1,-1,-1 }, stageBlockHModel_{ -1 }, stageHModel_{ -1 }, floorHModel_{ -1 }
    , decBoneCount_{ -1 }, isBoneDeath_{ false }, isBoneTatch_{ false }, number_{ 0 }, killTime_{ 9999 }, killTimeWait_{ 30 }, killTimeMax_{ 9999 }
    , gameState_{GAMESTATE::READY}
    , pParent_{ nullptr }, pPlayScene_{ nullptr }, pAttackPlayer_{ nullptr }, pCollision_{ nullptr }
    , pWoodBox_{ nullptr }, pText_{ nullptr }, pStage_{ nullptr }, pStageBlock_{ nullptr }, pFloor_{ nullptr }, pSceneManager_{ nullptr },pItemObjectManager_{nullptr},pStateManager_{nullptr}
{
    pParent_ = _pParent;
    //▼UIに関する基底クラスメンバ変数
    drawScoreTextX_ = 30;
    drawScoreTextY_ = 60;
    drawScoreNumberX_ = 360;
    drawScoreNumberY_ = 60;
    //▼ゲーム演出に関する基底クラスメンバ変数
    FPS_ = 600;
    timeCounter_ = 0;
    timeLimit_ = 60;
    fallLimit_ = 100.0f;
    score_ = 0;
    scoreAmount_ = 10;
    scoreMax_ = 150;
    padID_ = 1;
    playerInitPosY_ = 0.6f;
    //▼サウンドに関する基底クラスメンバ変数
    soundVolume_ = 0.5f;
    soundVolumeHalf_ = soundVolume_ / 2;
    //▼収集側プレイヤー移動に関する基底クラスメンバ変数
    CamPositionVec_ = {};
    positionPrev_ = { 0.0f,0.0f,0.0f };
    controllerMoveSpeed_ = { 0.3f,0.0f,0.3f };
    positionY_ = 0.0f;
    isDash_ = false;
    isFling_ = 1.0f;
    //▼向き変えに関する基底クラスメンバ変数
    vecMove_ = { 0.0f,0.0f,0.0f,0.0f };
    vecCross_ = { 0.0f,0.0f,0.0f,0.0f };
    vecDirection_ = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(collectPlayerNumber);
    angle_ = 0.0f;
    //▼収集側プレイヤージャンプに関する基底クラスメンバ変数
    gravity_ = 0.007f;
    positionTempY_ = 0.0f;
    positionPrevY_ = 0.0f;
    jumpPower_ = 0.3f;
    isJump_ = false;
    //▼飛びつきに関するメンバ変数
    divePower_ = 0.1f;
    diveSpeed_ = 0.6f;
    isDive_ = false;
    isDived_ = false;
    diveTime_ = 0;
    diveDuration_ = 1;
    diveTimeWait_ = 30;
    //▼すり抜け床に関する基底クラスメンバ変数
    isOnFloor_ = false;
    //▼木箱に関する基底クラスメンバ変数
    woodBoxNumber_ = "";
    dotProduct_ = 0.0f;
    angleDegrees_ = 0.0f;
    angleDegreesMax_ = 80.0f;
    //▼スタンに関する基底クラスメンバ変数
    stunTimeCounter_ = 0;
    stunLimit_ = 0;
    hitStopTime_ = 10;
    getUpTime_ = 30;
    knockbackSpeed_ = 0.3f;
    isStun_ = false;
    isKnockBack_ = false;
    //▼壁判定に関する基底クラスメンバ変数
    distMax_ = 99999.0f;
    inTheWall_ = 1.5f;
    rayFloorDistDown_ = 0.0f;
    rayStageDistDown_ = 0.0f;
}

CollectPlayer::~CollectPlayer()
{
}

void CollectPlayer::Initialize()
{
    //▼サウンドデータのロード
    std::string soundName;
    for (int i = initZeroInt; i < sizeof(soundCollectPlayerNames) / sizeof(soundCollectPlayerNames[initZeroInt]); i++)
    {
        soundName = soundFolderName + soundCollectPlayerNames[i] + soundModifierName;
        hSound_[i] = Audio::Load(soundName);
        assert(hSound_[i] >= initZeroInt);
    }
    //▼モデルデータのロード
    std::string modelName = modelFolderName + collectPlayerName + modelModifierName;
    hModel_ = Model::Load(modelName);
    assert(hModel_ >= initZeroInt);
    transform_.scale_ = { 0.4f,0.4f,0.4f};
    positionY_ = transform_.position_.y;
    pCollision_ = new SphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 2.0f);
    AddCollider(pCollision_);
    pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
    pPlayScene_ = (PlayScene*)FindObject(playSceneName);
    pStage_ = (Stage*)FindObject(stageName);      //ステージオブジェクト
    pStageBlock_ = (StageBlock*)FindObject(stageBlockName);
    pFloor_ = (Floor*)FindObject(floorName);
    pItemObjectManager_ = pPlayScene_->GetItemObjectManager();
    pStateManager_ = new StateManager(this);

    pStateManager_->AddState("WaitState", new PlayerWaitState(pStateManager_));
    pStateManager_->AddState("WalkState", new PlayerWalkState(pStateManager_));
    pStateManager_->AddState("RunState", new PlayerRunState(pStateManager_));
    pStateManager_->AddState("JumpState", new PlayerJumpState(pStateManager_));
    pStateManager_->AddState("StunState", new PlayerStunState(pStateManager_));
    pStateManager_->ChangeState("WaitState");
    pText_ = new Text;
    pText_->Initialize();
}

void CollectPlayer::Update()
{
    //ステートマネージャーの更新
    pStateManager_->Update();

    switch (gameState_)
    {
    case GAMESTATE::READY:          UpdateReady();      break;
    case GAMESTATE::PLAY:           UpdatePlay();       break;
    case GAMESTATE::GAMEOVER:       UpdateGameOver();   break;
    }
}

void CollectPlayer::Draw()
{
    pText_->Draw(drawScoreTextX_, drawScoreTextY_, "CollectPlayer:Score=");
    pText_->Draw(drawScoreNumberX_, drawScoreNumberY_, score_);

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void CollectPlayer::Release()
{
    SAFE_DELETE(pStateManager_);
}

void CollectPlayer::UpdateReady()
{
    RayCastData stageDataDown;
    stageHModel_ = pStage_->GetModelHandle();         //モデル番号を取得
    floorHModel_ = pFloor_->GetModelHandle();
    //▼下の法線(地面に張り付き)
    stageDataDown.start = transform_.position_;  //レイの発射位置
    stageDataDown.start.y = initZeroInt;
    XMStoreFloat3(&stageDataDown.dir, vecDown);     //レイの方向
    Model::RayCast(stageHModel_, &stageDataDown); //レイを発射
    rayStageDistDown_ = stageDataDown.dist;
    if (stageDataDown.hit)
    {
        transform_.position_.y = -stageDataDown.dist + playerInitPosY_;
    }
    ++timeCounter_;
    if (timeCounter_ >= timeLimit_)
    {
        gameState_ = GAMESTATE::PLAY;
        timeCounter_ = initZeroInt;
    }
    positionY_ = transform_.position_.y;
}

void CollectPlayer::UpdatePlay()
{
    PlayerBase::UpdatePlay();
    if (Input::IsKeyDown(DIK_D))
    {
        pPlayScene_->SetGameStop();
        gameState_ = GAMESTATE::GAMEOVER;
    }
    //落ちた時の処理
    if (transform_.position_.y <= -fallLimit_)
    {
        int revivalTime = 60;
        PlayerRevival();
        PlayerStun(revivalTime);
    }

    if (isDive_ && !isDived_)
    {
        ++diveTime_;
        if (diveTime_ <= diveDuration_)
        {
            PlayerDivePower();
        }
        PlayerDive();
    }

    PlayerFall();
    PlayerJump();
    PlayerRayCast();
    PlayerKnockback();
    transform_.position_.y = positionY_;
    if (isStun_)
    {
        stunTimeCounter_++;
        if (stunTimeCounter_ >= stunLimit_)
        {
            gameState_ = GAMESTATE::PLAY;
            isStun_ = false;
            isKnockBack_ = false;
            stunTimeCounter_ = initZeroInt;
        }
    }
    if (!isStun_)
    {
        PlayerMove();
    }
    if (score_ >= scoreMax_)
    {
        pPlayScene_->SetGameStop();
        gameState_ = GAMESTATE::GAMEOVER;
    }
    if (IsMoving() && !isJump_ && !isDash_)
    {
        Audio::Play(hSound_[((int)SOUNDSTATE::WALK)], soundVolume_);
    }
    if (!IsMoving() && !isJump_)
    {
        Audio::Stop(hSound_[((int)SOUNDSTATE::WALK)]);
        Audio::Stop(hSound_[((int)SOUNDSTATE::RUN)]);
    }
    if (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, padID_) && !isJump_ && IsMoving())
    {
        Audio::Stop(hSound_[((int)SOUNDSTATE::WALK)]);
        Audio::Play(hSound_[((int)SOUNDSTATE::RUN)], soundVolumeHalf_);
        isDash_ = true;
    }
    else
    {
        isDash_ = false;
    }
    if (isJump_)
    {
    }
    if (isStun_)
    {
    }
    if (isBoneTatch_)
    {
        if (killTime_ > initZeroInt)
        {
            --killTime_;
        }
    }

    if (killTime_ <= initZeroInt && isBoneTatch_)
    {
        score_ += scoreAmount_;
        pPlayScene_->AddBoneCount(decBoneCount_);
        isBoneDeath_ = true;
        isBoneTatch_ = false;
        Audio::Stop(hSound_[((int)SOUNDSTATE::CollectBone)]);
        killTime_ = killTimeMax_;
    }
}

void CollectPlayer::UpdateGameOver()
{
    Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::LEFTVIEW);
    if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, attackPlayerNumber) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, collectPlayerNumber))
    {
        pSceneManager_->ChangeScene(SCENE_ID_GAMEOVER);
        PlayerScore_[collectPlayerNumber] = this->GetScore();
        PlayerScore_[attackPlayerNumber] = pAttackPlayer_->GetScore();
    }
}

void CollectPlayer::PlayerWaitStateFunc()
{
    Model::SetAnimFrame(hModel_, 0, 0, 1.0f);
}

void CollectPlayer::PlayerWalkStateFunc()
{
    Model::SetAnimFrame(hModel_, 20, 60, 0.5f);
}

void CollectPlayer::PlayerRunStateFunc()
{
    Model::SetAnimFrame(hModel_, 80, 120, 0.5f);
}

void CollectPlayer::PlayerJumpStateFunc()
{
    Model::SetAnimFrame(hModel_, 120, 120, 1.0f);
}

void CollectPlayer::PlayerStunStateFunc()
{
    Model::SetAnimFrame(hModel_, 140, 200, 0.5f);
}

void CollectPlayer::PlayerStun(int _timeLimit)
{
    PlayerBase::PlayerStun(_timeLimit);
    Audio::Play(hSound_[((int)SOUNDSTATE::STUN)], soundVolume_);
}

void CollectPlayer::OnCollision(GameObject* _pTarget)
{
    std::vector<int> woodBoxs = pPlayScene_->GetWoodBoxs();
    woodBoxNumber_ = woodBoxName + std::to_string(number_);
    if (_pTarget->GetObjectName() == woodBoxNumber_)
    {
        pWoodBox_ = (WoodBox*)FindObject(woodBoxNumber_);
        XMVECTOR vecPos = XMLoadFloat3(&transform_.position_) - pWoodBox_->GetVecPos();
        vecPos = XMVector3Normalize(vecPos);
        dotProduct_ = XMVectorGetX(XMVector3Dot(vecPos, vecUp));
        float angleRadians = acosf(dotProduct_);
        angleDegrees_ = XMConvertToDegrees(angleRadians);
        if (angleDegrees_ <= angleDegreesMax_)
        {
            PlayerJumpPower();
            pWoodBox_->SetWoodBoxBreak();
            pPlayScene_->AddWoodBoxCount(-woodBoxDeath_);

        }
    }
    //WoodBoxという名前を持つ全てのオブジェクトを参照
    if (_pTarget->GetObjectName().find(woodBoxName) != std::string::npos)
    {
        if (angleDegrees_ > angleDegreesMax_)
        {
            transform_.position_ = positionPrev_;
        }
    }

    if (_pTarget->GetObjectName() == boneName && killTime_ == killTimeMax_)
    {
        SetKillTime(killTimeWait_);
        isBoneTatch_ = true;
        Audio::Play(hSound_[((int)SOUNDSTATE::CollectBone)]);
    }
    ++number_;
    if (number_ >= woodBoxs.size())
    {
        number_ = initZeroInt;
    }

    if (_pTarget->GetObjectName() == attackPlayerName)
    {
        PlayerStun(hitStopTime_);
        isKnockBack_ = true;
        vecKnockbackDirection_ = (XMLoadFloat3(&transform_.position_) - pAttackPlayer_->GetVecPos());
        vecKnockbackDirection_ = XMVector3Normalize(vecKnockbackDirection_);
    }
}

void CollectPlayer::PlayerFall()
{
    PlayerBase::PlayerFall();
}

void CollectPlayer::PlayerMove()
{
    vecDirection_ = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(collectPlayerNumber);
    PlayerBase::PlayerMove();
    if (!(Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER, padID_)))
    {
        XMVECTOR vecCam = {};
        CamPositionVec_ = Camera::VecGetPosition(collectPlayerNumber);
        vecCam = -(CamPositionVec_ - Camera::VecGetTarget(collectPlayerNumber));
        XMFLOAT3 camRot = {};
        XMStoreFloat3(&camRot, vecCam);
        camRot.y = 0;
        vecCam = XMLoadFloat3(&camRot);
        vecCam = XMVector3Normalize(vecCam);
        vecMove_ = vecCam;
    }

    if (Input::IsPadButton(XINPUT_GAMEPAD_A, padID_) && !isJump_)
    {
        PlayerJumpPower();
        Audio::Stop(hSound_[((int)SOUNDSTATE::WALK)]);
        Audio::Stop(hSound_[((int)SOUNDSTATE::RUN)]);
        Audio::Play(hSound_[((int)SOUNDSTATE::JUMP)], soundVolumeHalf_);
    }
}

void CollectPlayer::PlayerJump()
{

}

void CollectPlayer::PlayerJumpPower()
{
    PlayerBase::PlayerJumpPower();
}

void CollectPlayer::PlayerDive()
{
    PlayerBase::PlayerDive();
}

void CollectPlayer::PlayerDivePower()
{
    //ダイブの処理
    PlayerBase::PlayerDivePower();
}

void CollectPlayer::PlayerKnockback()
{
    PlayerBase::PlayerKnockback();
}

void CollectPlayer::PlayerRayCast()
{
    float rayFloorDistUp = 0.0f;
    float rayStageBlockDistDown = 0.0f;
    float rayStageDistFront = 0.0f;
    float rayStageDistBack = 0.0f;
    float rayStageDistLeft = 0.0f;
    float rayStageDistRight = 0.0f;
    RayCastData floorDataUp;
    RayCastData floorDataDown;
    RayCastData stageBlockDataDown;
    RayCastData stageDataDown;
    RayCastData stageDataFront;
    RayCastData stageDataBack;
    RayCastData stageDataLeft;
    RayCastData stageDataRight;                 //プレイヤーが地面からどのくらい離れていたら浮いている判定にするか
    stageHModel_ = pStage_->GetModelHandle();         //モデル番号を取得
    floorHModel_ = pFloor_->GetModelHandle();

    for (int i = 0; i < pItemObjectManager_->GetFloors().size(); i++)
    {
        //▼上の法線(すり抜け床のため)
        floorDataUp.start = transform_.position_;           //レイの発射位置
        XMStoreFloat3(&floorDataUp.dir, vecUp);             //レイの方向
        Model::RayCast(floorHModel_ + i, &floorDataUp);         //レイを発射
        rayFloorDistUp = floorDataUp.dist;

        //▼下の法線(すり抜け床)
        floorDataDown.start = transform_.position_;    //レイの発射位置
        floorDataDown.start.y = 0;
        XMStoreFloat3(&floorDataDown.dir, vecDown);     //レイの方向
        if (floorDataUp.dist == distMax_)
        {
            Model::RayCast(floorHModel_ + i, &floorDataDown);  //レイを発射
        }
        rayFloorDistDown_ = floorDataDown.dist;
        if (rayFloorDistDown_ + positionY_ <= isFling_)
        {
            if (!isJump_)
            {
                isOnFloor_ = true;
                isDived_ = false;
                positionY_ = -floorDataDown.dist + playerInitPosY_;
                positionTempY_ = positionY_;
                positionPrevY_ = positionTempY_;
            }
        }
        else
        {
            isOnFloor_ = false;
        }

    }

    //▼下の法線(床に張り付き)
    stageDataDown.start = transform_.position_;     //レイの発射位置
    stageDataDown.start.y = initZeroFloat;
    XMStoreFloat3(&stageDataDown.dir, vecDown);     //レイの方向
    Model::RayCast(stageHModel_, &stageDataDown);   //レイを発射
    rayStageDistDown_ = stageDataDown.dist;
    //プレイヤーが浮いていないとき
    if (rayStageDistDown_ + positionY_ <= isFling_)
    {
        //ジャンプしてない＆すり抜け床の上にいない
        if (!isJump_ && !isOnFloor_)
        {
            //地面に張り付き
            isDived_ = false;
            positionY_ = -stageDataDown.dist + playerInitPosY_;
            positionTempY_ = positionY_;
            positionPrevY_ = positionTempY_;
        }
    }
    else if (!isOnFloor_)
    {
        isJump_ = true;
    }
    //▼前の法線(壁の当たり判定)
    stageDataFront.start = transform_.position_;      //レイの発射位置
    XMStoreFloat3(&stageDataFront.dir, vecFrontUp);   //レイの方向
    Model::RayCast(stageHModel_, &stageDataFront);    //レイを発射
    rayStageDistFront = stageDataFront.dist;
    if (rayStageDistFront <= inTheWall_)
    {
        transform_.position_.z = positionPrev_.z;
    }
    //▼後ろの法線(壁の当たり判定)
    stageDataBack.start = transform_.position_;       //レイの発射位置
    XMStoreFloat3(&stageDataBack.dir, vecBackUp);     //レイの方向
    Model::RayCast(stageHModel_, &stageDataBack);     //レイを発射
    rayStageDistBack = stageDataBack.dist;
    if (rayStageDistBack <= inTheWall_)
    {
        transform_.position_.z = positionPrev_.z;
    }
    //▼左の法線(壁の当たり判定)
    stageDataLeft.start = transform_.position_;       //レイの発射位置
    XMStoreFloat3(&stageDataLeft.dir, vecLeftUp);     //レイの方向
    Model::RayCast(stageHModel_, &stageDataLeft);     //レイを発射
    rayStageDistLeft = stageDataLeft.dist;
    if (rayStageDistLeft <= inTheWall_)
    {
        transform_.position_.x = positionPrev_.x;
    }
    //▼右の法線(壁の当たり判定)
    stageDataRight.start = transform_.position_;       //レイの発射位置
    XMStoreFloat3(&stageDataRight.dir, vecRightUp);    //レイの方向
    Model::RayCast(stageHModel_, &stageDataRight);     //レイを発射
    rayStageDistRight = stageDataRight.dist;
    if (rayStageDistRight <= inTheWall_)
    {
        transform_.position_.x = positionPrev_.x;
    }
    positionPrev_ = transform_.position_;
}

void CollectPlayer::PlayerRevival()
{
    PlayerBase::PlayerRevival();
}

void CollectPlayer::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
    transform_.position_.x = transform_.position_.x + _knockbackSpeed * XMVectorGetX(_vecKnockbackDirection);
    transform_.position_.z = transform_.position_.z + _knockbackSpeed * XMVectorGetZ(_vecKnockbackDirection);
}

bool CollectPlayer::IsMoving()
{
    return (transform_.position_.x != positionPrev_.x || transform_.position_.z != positionPrev_.z);
}