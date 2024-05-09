//インクルード
#include "../Engine/Input.h"
#include "../Engine/Model.h"
#include "../Engine/Direct3D.h"
#include "../Engine/ImGui/imgui.h"
#include "../Engine/Text.h"
#include "../Engine/Audio.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Global.h"
#include "../Engine/SceneManager.h"
#include "../ItemObject/Floor.h"
#include "../ItemObject/WoodBox.h"
#include "../ItemObject/ItemObjectManager.h"
#include "../StageObject/Stage.h"
#include "../State/StateManager.h"
#include "../State/AttackPlayerState.h"
#include "AttackPlayer.h"
#include "CollectPlayer.h"

AttackPlayer::AttackPlayer(GameObject* _pParent)
    :PlayerBase(_pParent, attackPlayerName), hModel_{ -1 }, hSound_{ -1,-1,-1,-1 }, stageHModel_{0}, floorHModel_{0}
    , number_{ 0 },  scoreTimeCounter_{ 0 }, scoreTimeCounterWait_{ 1 }
    , gameState_{ GAMESTATE::READY }
    , pParent_{ nullptr }, pPlayScene_{ nullptr }, pCollectPlayer_{ nullptr }, pCollision_{ nullptr }
    , pWoodBox_{ nullptr }, pText_{ nullptr }, pStage_{ nullptr }, pFloor_{ nullptr }, pSceneManager_{ nullptr }, pItemObjectManager_{nullptr}, pStateManager_{nullptr}
{
    pParent_ = _pParent;
    //▼UIに関する基底クラスメンバ変数
    drawScoreTextX_ = 30;
    drawScoreTextY_ = 30;
    drawScoreNumberX_ = 360;
    drawScoreNumberY_ = 30;
    //▼ゲーム演出に関する基底クラスメンバ変数
    FPS_ = 60 * 10;
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
    //▼邪魔側プレイヤー移動に関する基底クラスメンバ変数
    CamPositionVec_ = {};
    positionPrev_ = { 0.0f,0.0f,0.0f };
    controllerMoveSpeed_ = { 0.3f,0.0f,0.3f };
    positionY_ = 0.0f;
    isRun_ = false;
    isFling_ = 1.0f;
    //▼向き変えに関する基底クラスメンバ変数
    vecMove_ = { 0.0f,0.0f,0.0f,0.0f };
    vecCross_ = { 0.0f,0.0f,0.0f,0.0f };
    angle_ = 0.0f;
    //▼邪魔側プレイヤージャンプに関する基底クラスメンバ変数
    gravity_ = 0.007f;
    positionTempY_ = 0.0f;
    positionPrevY_ = 0.0f;
    jumpPower_ = 0.3f;
    isJump_ = false;
    //▼飛びつきに関する基底クラスメンバ変数
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
    //▼アニメーションに関する基底クラスメンバ変数
    startFrame_ = 0;
    endFrame_ = 0;
    animSpeed_ = 0.0f;
}

AttackPlayer::~AttackPlayer()
{
}

void AttackPlayer::Initialize()
{
    //▼INIファイルからデータのロード
    attackOrCollect_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollect", 0, "Setting/PlayerSetting.ini");
    //▼サウンドデータのロード
    std::string soundName;
    for (int i = 0; i < sizeof(soundAttackPlayerNames) / sizeof(soundAttackPlayerNames[initZeroInt]); i++)
    {
        soundName = soundFolderName + soundAttackPlayerNames[i] + soundModifierName;
        hSound_[i] = Audio::Load(soundName);
        assert(hSound_[i] >= 0);
    }
    //▼モデルデータのロード
    std::string modelName = modelFolderName + attackPlayerName + modelModifierName;
    hModel_ = Model::Load(modelName);
    assert(hModel_ >= 0);
    transform_.scale_ = { 0.4f,0.4f,0.4f };
    positionY_ = transform_.position_.y;
    pCollision_ = new SphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 2.0f);
    AddCollider(pCollision_);
    pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
    pPlayScene_ = (PlayScene*)FindObject(playSceneName);
    pStage_ = (Stage*)FindObject(stageName);
    pFloor_ = (Floor*)FindObject(floorName);
    pItemObjectManager_ = pPlayScene_->GetItemObjectManager();
    pStateManager_ = new StateManager(this);
    pStateManager_->AddState("WalkState", new AttackPlayerWalkState(pStateManager_));
    pStateManager_->AddState("WaitState", new AttackPlayerWaitState(pStateManager_));
    pStateManager_->AddState("RunState", new AttackPlayerRunState(pStateManager_));
    pStateManager_->AddState("JumpState", new AttackPlayerJumpState(pStateManager_));
    pStateManager_->AddState("StunState", new AttackPlayerStunState(pStateManager_));
    pStateManager_->ChangeState("WaitState");

    pText_ = new Text;
    pText_->Initialize();

    if (attackOrCollect_ == (int)PADIDSTATE::FIRST)
    {
        padID_ = (int)PADIDSTATE::FIRST;
    }
    if (attackOrCollect_ == (int)PADIDSTATE::SECONDS)
    {
        padID_ = (int)PADIDSTATE::SECONDS;
    }
    vecDirection_ = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(padID_);
}

void AttackPlayer::Update()
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

void AttackPlayer::Draw()
{
    pText_->Draw(drawScoreTextX_, drawScoreTextY_, "AttackPlayer:Score=");
    pText_->Draw(drawScoreNumberX_, drawScoreNumberY_, score_);

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void AttackPlayer::Release()
{
    SAFE_DELETE(pStateManager_);
}

void AttackPlayer::UpdateReady()
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

void AttackPlayer::UpdatePlay()
{
    PlayerBase::UpdatePlay();
    if (Input::IsKeyDown(DIK_A))
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
    scoreTimeCounter_++;
    if (scoreTimeCounter_ % FPS_ == scoreTimeCounterWait_)
    {
        score_ += scoreAmount_;
    }
    PlayerFall();
    PlayerRayCast();
    PlayerKnockback();
    transform_.position_.y = positionY_;
    if (isStun_)
    {
        stunTimeCounter_++;
        if (stunTimeCounter_ >= stunLimit_)
        {
            isStun_ = false;
            isKnockBack_ = false;
            gameState_ = GAMESTATE::PLAY;
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
    if (isMove_ && !isJump_ && !isRun_)
    {
        Audio::Play(hSound_[((int)SOUNDSTATE::WALK)], soundVolume_);
    }
    if (!isMove_ && !isJump_)
    {
        Audio::Stop(hSound_[((int)SOUNDSTATE::WALK)]);
        Audio::Stop(hSound_[((int)SOUNDSTATE::RUN)]);
    }
    if (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER,padID_) && !isJump_ && isMove_)
    {
        Audio::Stop(hSound_[((int)SOUNDSTATE::WALK)]);
        Audio::Play(hSound_[((int)SOUNDSTATE::RUN)], soundVolumeHalf_);
    }
    IsMove();
    IsJump();
    IsRun();
    IsStun();
    IsDive();
}

void AttackPlayer::UpdateGameOver()
{
    Direct3D::SetIsChangeView(((int)Direct3D::VIEWSTATE::RIGHTVIEW));
    if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, padID_))
    {
        pSceneManager_->ChangeScene(SCENE_ID_GAMEOVER);
    }
}

void AttackPlayer::PlayerWaitStateFunc()
{
    PlayerBase::PlayerWaitStateFunc();
    Model::SetAnimFrame(hModel_, startFrame_, endFrame_, animSpeed_);
}

void AttackPlayer::PlayerWalkStateFunc()
{
    PlayerBase::PlayerWalkStateFunc();
    Model::SetAnimFrame(hModel_, startFrame_, endFrame_, animSpeed_);
}

void AttackPlayer::PlayerRunStateFunc()
{
    PlayerBase::PlayerRunStateFunc();
    Model::SetAnimFrame(hModel_, startFrame_, endFrame_, animSpeed_);
}

void AttackPlayer::PlayerJumpStateFunc()
{
    PlayerBase::PlayerJumpStateFunc();
    Model::SetAnimFrame(hModel_, startFrame_, endFrame_, animSpeed_);
}

void AttackPlayer::PlayerStunStateFunc()
{
    PlayerBase::PlayerStunStateFunc();
    Model::SetAnimFrame(hModel_, startFrame_, endFrame_, animSpeed_);
}

void AttackPlayer::PlayerStun(int _timeLimit)
{
    PlayerBase::PlayerStun(_timeLimit);
}

void AttackPlayer::OnCollision(GameObject* _pTarget)
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
    ++number_;
    if (number_ >= woodBoxs.size())
    {
        number_ = initZeroInt;
    }

    if (_pTarget->GetObjectName() == collectPlayerName)
    {
        PlayerStun(hitStopTime_);
        isKnockBack_ = true;
        vecKnockbackDirection_ = (XMLoadFloat3(&transform_.position_) - pCollectPlayer_->GetVecPos());
        vecKnockbackDirection_ = XMVector3Normalize(vecKnockbackDirection_);
    }
}

void AttackPlayer::PlayerFall()
{
    PlayerBase::PlayerFall();
}

void AttackPlayer::PlayerMove()
{
    vecDirection_ = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(padID_);
    PlayerBase::PlayerMove();
    if (!(Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER, padID_)))
    {
        XMVECTOR vecCam = {};
        CamPositionVec_ = Camera::VecGetPosition(padID_);
        vecCam = -(CamPositionVec_ - Camera::VecGetTarget(padID_));
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
        Audio::Play(hSound_[((int)SOUNDSTATE::JUMP)],soundVolumeHalf_);
    }
}

void AttackPlayer::PlayerJump()
{

}

void AttackPlayer::PlayerJumpPower()
{
    PlayerBase::PlayerJumpPower();
}

void AttackPlayer::PlayerDive()
{
    PlayerBase::PlayerDive();
}

void AttackPlayer::PlayerDivePower()
{
    //とびつきの処理
    PlayerBase::PlayerDivePower();
}

void AttackPlayer::PlayerKnockback()
{
    PlayerBase::PlayerKnockback();
}

void AttackPlayer::PlayerRayCast()
{
    float rayFloorDistUp = 0.0f;
    float rayStageBlockDistDown = 0.0f;
    float rayStageDistFront = 0.0f;
    float rayStageDistBack = 0.0f;
    float rayStageDistLeft = 0.0f;
    float rayStageDistRight = 0.0f;
    RayCastData floorDataUp;
    RayCastData floorDataDown;
    RayCastData stageDataDown;
    RayCastData stageDataFront;
    RayCastData stageDataBack;
    RayCastData stageDataLeft;
    RayCastData stageDataRight;                       //プレイヤーが地面からどのくらい離れていたら浮いている判定にするか
    stageHModel_ = pStage_->GetModelHandle();         //モデル番号を取得
    floorHModel_ = pFloor_->GetModelHandle();

    for (int i = 0;i < pItemObjectManager_->GetFloors().size(); i++)
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
        if (rayFloorDistUp == distMax_)
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
                positionY_ = -rayFloorDistDown_ + playerInitPosY_;
                positionTempY_ = positionY_;
                positionPrevY_ = positionTempY_;
            }
        }
        else
        {
            isOnFloor_ = false;
        }

    }
    //▼下の法線(地面に張り付き)
    stageDataDown.start = transform_.position_;   //レイの発射位置
    stageDataDown.start.y = initZeroFloat;
    XMStoreFloat3(&stageDataDown.dir, vecDown);   //レイの方向
    Model::RayCast(stageHModel_, &stageDataDown); //レイを発射
    rayStageDistDown_ = stageDataDown.dist;
    //プレイヤーが浮いていないとき
    if (rayStageDistDown_ + positionY_ <= isFling_)
    {
        //ジャンプしてない＆すり抜け床の上にいない
        if (!isJump_ && !isOnFloor_)
        {
            //地面に張り付き
            isDived_ = false;
            positionY_ = -rayStageDistDown_ + playerInitPosY_;
            positionTempY_ = positionY_;
            positionPrevY_ = positionTempY_;
        }
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
    stageDataRight.start = transform_.position_;      //レイの発射位置
    XMStoreFloat3(&stageDataRight.dir, vecRightUp);   //レイの方向
    Model::RayCast(stageHModel_, &stageDataRight);    //レイを発射
    rayStageDistRight = stageDataRight.dist;
    if (rayStageDistRight <= inTheWall_)
    {
        transform_.position_.x = positionPrev_.x;
    }
    positionPrev_ = transform_.position_;
}

void AttackPlayer::PlayerRevival()
{
    PlayerBase::PlayerRevival();
}

void AttackPlayer::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
    transform_.position_.x = transform_.position_.x + _knockbackSpeed * XMVectorGetX(_vecKnockbackDirection);
    transform_.position_.z = transform_.position_.z + _knockbackSpeed * XMVectorGetZ(_vecKnockbackDirection);
}

void AttackPlayer::IsMove()
{
    PlayerBase::IsMove();
}

void AttackPlayer::IsJump()
{
    PlayerBase::IsJump();
}

void AttackPlayer::IsRun()
{
    PlayerBase::IsRun();
}

void AttackPlayer::IsStun()
{
    PlayerBase::IsStun();
}

void AttackPlayer::IsDive()
{
    PlayerBase::IsDive();
}
