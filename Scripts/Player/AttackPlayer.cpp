//インクルード
#include "../../Engine/Input.h"
#include "../../Engine/Model.h"
#include "../../Engine/Direct3D.h"
#include "../../Engine/ImGui/imgui.h"
#include "../../Engine/Text.h"
#include "../../Engine/Audio.h"
#include "../../Engine/SceneManager.h"
#include "../../Engine/Global.h"
#include "../ItemObject/Floor.h"
#include "../ItemObject/WoodBox.h"
#include "../ItemObject/Bone.h"
#include "../ItemObject/BoneSuck.h"
#include "../ItemObject/ItemObjectManager.h"
#include "../StageObject/Stage.h"
#include "../State/StateManager.h"
#include "../State/AttackPlayerState.h"
#include "../Scene/Dogs_Walk_PlayScene.h"
#include "../Scene/Dogs_Fight_PlayScene.h"
#include "../ImageManager.h"
#include "../ParticleManager.h"
#include "../UIManager.h"
#include "AttackPlayer.h"
#include "CollectPlayer.h"
#include "AIPlayer.h"

using enum PlayerBase::GAMESTATE;
using enum PLAYSCENESTATE;
using enum PADIDSTATE;
using enum ImageManager::IMAGESTATE;
using enum MOUSESTATE;
using enum AttackPlayer::SOUNDSTATE;
using enum Direct3D::VIEWSTATE;

AttackPlayer::AttackPlayer(GameObject* _pParent)
    :PlayerBase(_pParent, attackPlayerName), hModel_{ -1 }, hSound_{ -1,-1,-1,-1 }, stageHModel_{-1}, floorHModel_{-1}
    ,number_{0}, gameState_{ GAMEREADY }, attackOrCollect_{ 0 }
    , pParent_{ _pParent }, pDogs_Walk_PlayScene_{ nullptr },pDogs_Fight_PlayScene_{nullptr}, pCollectPlayer_{nullptr},pAIPlayer_{nullptr}, pCollision_{nullptr}
    , pWoodBox_{ nullptr },pBoneSuck_{nullptr}, pStage_{nullptr}, pFloor_{nullptr}
    , pSceneManager_{nullptr}, pItemObjectManager_{nullptr}, pStateManager_{nullptr},pImageManager_{nullptr}
, pBoneImageManager_{ nullptr }, pParticleManager_{ nullptr }, slowTime_{0},slowTimeWait_{1}
{
}

AttackPlayer::~AttackPlayer()
{
}

void AttackPlayer::Initialize()
{
    //▼INIファイルからデータのロード
    attackOrCollect_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollect", 0, "Setting/PlayerSetting.ini");
    gameData_.walkOrFight_ = GetPrivateProfileInt("PLAYSCENEID", "WalkOrFight", 0, "Setting/PlaySceneSetting.ini");
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
    jumpData_.positionY_ = transform_.position_.y;
    if (gameData_.walkOrFight_ == (int)DOGSFIGHT)
    {
        gameData_.FPS_ = 300;
    }
    //▼コリジョンを設定
    pCollision_ = new SphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 2.0f);
    AddCollider(pCollision_);
    //▼ポインタ変数に実体を設定
    pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
    pDogs_Walk_PlayScene_ = (Dogs_Walk_PlayScene*)FindObject(Dogs_Walk_PlaySceneName);
    pDogs_Fight_PlayScene_ = (Dogs_Fight_PlayScene*)FindObject(Dogs_Fight_PlaySceneName);
    pStage_ = (Stage*)FindObject(stageName);
    pFloor_ = (Floor*)FindObject(floorName);
    if (gameData_.walkOrFight_ == (int)DOGSWALK)
    {
        pItemObjectManager_ = pDogs_Walk_PlayScene_->GetItemObjectManager();
    }
    if (gameData_.walkOrFight_ == (int)DOGSFIGHT)
    {
        pItemObjectManager_ = pDogs_Fight_PlayScene_->GetItemObjectManager();
    }
    pStateManager_ = new StateManager(this);
    pStateManager_->AddState("WalkState", new AttackPlayerWalkState(pStateManager_));
    pStateManager_->AddState("WaitState", new AttackPlayerWaitState(pStateManager_));
    pStateManager_->AddState("RunState", new AttackPlayerRunState(pStateManager_));
    pStateManager_->AddState("JumpState", new AttackPlayerJumpState(pStateManager_));
    pStateManager_->AddState("StunState", new AttackPlayerStunState(pStateManager_));
    pStateManager_->ChangeState("WaitState");
    if (attackOrCollect_ == (int)FIRST)
    {
        gameData_.padID_ = (int)FIRST;
    }
    if (attackOrCollect_ == (int)SECONDS)
    {
        gameData_.padID_ = (int)SECONDS;
    }
    dirData_.vecDirection_ = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(gameData_.padID_);
    pParticleManager_ = Instantiate<ParticleManager>(this);
    pImageManager_ = Instantiate<ImageManager>(this);
    pImageManager_->SetMode((int)NONEIMAGE);
    pImageManager_->SecInit();
    pBoneImageManager_ = Instantiate<ImageManager>(this);
    pBoneImageManager_->SetMode((int)BONEIMAGE);
}

void AttackPlayer::Update()
{
    //▼ステートマネージャーの更新
    pStateManager_->Update();
    switch (gameState_)
    {
    case GAMEREADY:          UpdateReady();      break;
    case GAMEPLAY:           UpdatePlay();       break;
    case GAMEOVER:           UpdateGameOver();   break;
    }
}

void AttackPlayer::UpdateSlow()
{
}

void AttackPlayer::BothViewDraw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void AttackPlayer::LeftViewDraw()
{

}

void AttackPlayer::RightViewDraw()
{

}

void AttackPlayer::UPSubViewDraw()
{

}

void AttackPlayer::Release()
{
    SAFE_DELETE(pStateManager_);
}

void AttackPlayer::UpdateReady()
{
    PlayerCamera();
    RayCastData stageDataDown;
    stageHModel_ = pStage_->GetModelHandle();         //モデル番号を取得
    floorHModel_ = pFloor_->GetModelHandle();
    //▼下の法線(地面に張り付き)
    stageDataDown.start = transform_.position_;  //レイの発射位置
    stageDataDown.start.y = initZeroInt;
    XMStoreFloat3(&stageDataDown.dir, vecDown);     //レイの方向
    Model::RayCast(stageHModel_, &stageDataDown); //レイを発射
    wallData_.rayStageDistDown_ = stageDataDown.dist;
    if (stageDataDown.hit)
    {
        transform_.position_.y = -stageDataDown.dist + jumpData_.playerInitPosY_;
    }
    ++gameData_.timeCounter_;
    if (gameData_.timeCounter_ >= gameData_.timeLimit_)
    {
        gameState_ = GAMEPLAY;
        gameData_.timeCounter_ = initZeroInt;
    }
    jumpData_.positionY_ = transform_.position_.y;
}

void AttackPlayer::UpdatePlay()
{
    PlayerCamera();
    PlayerFall();
    PlayerRayCast();
    PlayerKnockback();
    PlayerBase::UpdatePlay();
    if (Input::IsKeyDown(DIK_A))
    {
        if (gameData_.walkOrFight_ == (int)DOGSWALK)
        {
            pDogs_Walk_PlayScene_->SetGameStop();
        }
        if (gameData_.walkOrFight_ == (int)DOGSFIGHT)
        {
            pDogs_Fight_PlayScene_->SetGameStop();
        }
        gameState_ = GAMEOVER;
    }
    //▼落ちた時の処理
    if (transform_.position_.y <= -gameData_.fallLimit_)
    {
        PlayerRevival();
        PlayerStun(revivalTime);
        if (pBoneSuck_ != nullptr)
        {
            pBoneSuck_->SetKillTime(boneData_.killTimeWait_);
        }
        SetKillTime(boneData_.killTimeWait_);
    }

    if (diveData_.isDive_ && !diveData_.isDived_)
    {
        ++diveData_.diveTime_;
        if (diveData_.diveTime_ <= diveData_.diveDuration_)
        {
            PlayerDivePower();
        }
        PlayerDive();
    }
    gameData_.scoreTimeCounter_++;
    if (gameData_.walkOrFight_ == (int)DOGSWALK)
    {
        if (gameData_.scoreTimeCounter_ % gameData_.FPS_ == gameData_.scoreTimeCounterWait_ && gameData_.scoreTimeCounter_ != gameData_.scoreTimeCounterWait_)
        {
    		PlayerAddScore();
        }
    }
    if (gameData_.walkOrFight_ == (int)DOGSFIGHT)
    {
        if (gameData_.scoreTimeCounter_ % gameData_.FPS_ == gameData_.scoreTimeCounterWait_ && boneData_.isBoneTatch_ && gameData_.scoreTimeCounter_ != gameData_.scoreTimeCounterWait_)
        {
            PlayerAddScore();
        }
    }
    transform_.position_.y = jumpData_.positionY_;
    if (stunData_.isStun_)
    {
        stunData_.stunTimeCounter_++;
        if (stunData_.stunTimeCounter_ >= stunData_.stunLimit_)
        {
            slowTime_ = 0;
            stunData_.isStun_ = false;
            stunData_.isKnockBack_ = false;
            gameState_ = GAMEPLAY;
            stunData_.stunTimeCounter_ = initZeroInt;
        }
    }
    slowTime_++;
    if (slowTime_ == slowTimeWait_)
    {
        UpdateSlow();
        slowTime_ = 0;
    }
    if (!stunData_.isStun_)
    {
        PlayerMove();
    }
    if (gameData_.score_ >= gameData_.scoreMax_)
    {
        if (gameData_.walkOrFight_ == (int)DOGSWALK)
        {
            pDogs_Walk_PlayScene_->SetGameStop();
        }
        if (gameData_.walkOrFight_ == (int)DOGSFIGHT)
        {
            pDogs_Fight_PlayScene_->SetGameStop();
        }
        gameState_ = GAMEOVER;
    }
    if (moveData_.isMove_ && !jumpData_.isJump_ && !moveData_.isRun_)
    {
        Audio::Play(hSound_[((int)WALK)], soundData_.soundVolume_);
    }
    if (!moveData_.isMove_ && !jumpData_.isJump_)
    {
        Audio::Stop(hSound_[((int)WALK)]);
        Audio::Stop(hSound_[((int)RUN)]);
    }
    if (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, gameData_.padID_) && !jumpData_.isJump_ && moveData_.isMove_)
    {
        Audio::Stop(hSound_[((int)WALK)]);
        Audio::Play(hSound_[((int)RUN)], soundData_.soundVolumeHalf_);
    }

    if (boneData_.isBoneTatch_)
    {
        if (boneData_.killTime_ > 0)
        {
            --boneData_.killTime_;
        }
    }

    if (boneData_.killTime_ <= 0 && boneData_.isBoneTatch_)
    {
        PlayerAddScore();
        pDogs_Fight_PlayScene_->AddBoneCount(boneData_.decBoneCount_);
        boneData_.isBoneTatch_ = false;
        Audio::Stop(hSound_[((int)COLLECTBONE)]);
        boneData_.killTime_ = boneData_.killTimeMax_;
    }
    IsMove();
    IsJump();
    IsRun();
    IsStun();
    IsDive();
}

void AttackPlayer::UpdateGameOver()
{
    pImageManager_->SetMode((int)GAMETITLEIMAGE);
    pImageManager_->SecInit();
    if (gameData_.padID_ == (int)FIRST)
    {
        Direct3D::SetIsChangeView(((int)LEFTVIEW));
	}
    if (gameData_.padID_ == (int)SECONDS)
    {
        Direct3D::SetIsChangeView(((int)RIGHTVIEW));
    }
    if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)FIRST) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)SECONDS))
    {
        //▼INIファイルへの書き込み
        WritePrivateProfileString("PLAYERSCORE", "AttackPlayerScore", std::to_string(gameData_.score_).c_str(), "Setting/PlayerSetting.ini");
        WritePrivateProfileString("PLAYERSCORE", "CollectPlayerScore", std::to_string(pCollectPlayer_->GetScore()).c_str(), "Setting/PlayerSetting.ini");
        pSceneManager_->ChangeScene(SCENE_ID_GAMEOVER);
    }
}

void AttackPlayer::PlayerWaitStateFunc()
{
    PlayerBase::PlayerWaitStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AttackPlayer::PlayerWalkStateFunc()
{
    PlayerBase::PlayerWalkStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AttackPlayer::PlayerRunStateFunc()
{
    PlayerBase::PlayerRunStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AttackPlayer::PlayerJumpStateFunc()
{
    PlayerBase::PlayerJumpStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AttackPlayer::PlayerStunStateFunc()
{
    PlayerBase::PlayerStunStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AttackPlayer::PlayerStun(int _timeLimit)
{
    PlayerBase::PlayerStun(_timeLimit);
}

void AttackPlayer::PlayerOuterWall()
{
    PlayerBase::PlayerOuterWall();
}

void AttackPlayer::OnCollision(GameObject* _pTarget)
{
    std::vector<int> woodBoxs = {};
    if (gameData_.walkOrFight_ == (int)DOGSWALK)
    {
        woodBoxs = pDogs_Walk_PlayScene_->GetWoodBoxs();
    }
    if (gameData_.walkOrFight_ == (int)DOGSFIGHT)
    {
        woodBoxs = pDogs_Fight_PlayScene_->GetWoodBoxs();
    }
    woodBoxData_.woodBoxNumber_ = woodBoxName + std::to_string(number_);
    if (_pTarget->GetObjectName() == woodBoxData_.woodBoxNumber_)
    {
        pWoodBox_ = (WoodBox*)FindObject(woodBoxData_.woodBoxNumber_);
        XMVECTOR vecPos = XMLoadFloat3(&transform_.position_) - pWoodBox_->GetVecPos();
        vecPos = XMVector3Normalize(vecPos);
        woodBoxData_.dotProduct_ = XMVectorGetX(XMVector3Dot(vecPos, vecUp));
        float angleRadians = acosf(woodBoxData_.dotProduct_);
        woodBoxData_.angleDegrees_ = XMConvertToDegrees(angleRadians);
        if (woodBoxData_.angleDegrees_ <= woodBoxData_.angleDegreesMax_)
        {
            PlayerJumpPower();
            pWoodBox_->SetWoodBoxBreak();
            if (gameData_.walkOrFight_ == (int)DOGSWALK)
            {
                pDogs_Walk_PlayScene_->AddWoodBoxCount(-woodBoxDeath_);
            }
            if (gameData_.walkOrFight_ == (int)DOGSFIGHT)
            {
                pDogs_Fight_PlayScene_->AddWoodBoxCount(-woodBoxDeath_);
            }
        }
    }
    //WoodBoxという名前を持つ全てのオブジェクトを参照
    if (_pTarget->GetObjectName().find(woodBoxName) != std::string::npos)
    {
        if (woodBoxData_.angleDegrees_ > woodBoxData_.angleDegreesMax_)
        {
            transform_.position_ = moveData_.positionPrev_;
        }
    }
    if (_pTarget->GetObjectName() == boneName)
    {
        if (boneData_.killTime_ == boneData_.killTimeMax_ && gameData_.walkOrFight_ == (int)DOGSFIGHT)
        {
            pParticleManager_->CreateVFX(transform_.position_);
            //Audio::Play(hSound_[((int)SOUNDSTATE::CollectBone)]);

            pBoneSuck_ = Instantiate<BoneSuck>(this);;
            SetKillTime(boneData_.killTimeWait_);
            static int noDeathBoneSuck = 99999;
            pBoneSuck_->SetKillTime(noDeathBoneSuck);
            SetKillTime(noDeathBoneSuck);
            boneData_.isBoneTatch_ = true;
            _pTarget->KillMe();
        }
    }
    ++number_;
    if (number_ >= woodBoxs.size())
    {
        number_ = initZeroInt;
    }

    if (_pTarget->GetObjectName() == collectPlayerName)
    {
        PlayerStun(stunData_.stunTime_);
        stunData_.isKnockBack_ = true;
        stunData_.vecKnockbackDirection_ = (XMLoadFloat3(&transform_.position_) - pCollectPlayer_->GetVecPos());
        stunData_.vecKnockbackDirection_ = XMVector3Normalize(stunData_.vecKnockbackDirection_);
        if (boneData_.killTime_ < boneData_.killTimeMax_)
        {
            if (pBoneSuck_ != nullptr)
            {
                pBoneSuck_->SetKillTime(boneData_.killTimeWait_);
            }
            SetKillTime(boneData_.killTimeWait_);
        }
    }

    if (_pTarget->GetObjectName() == aIPlayerName)
    {
        PlayerStun(stunData_.stunTime_);
        stunData_.isKnockBack_ = true;
        stunData_.vecKnockbackDirection_ = (XMLoadFloat3(&transform_.position_) - pAIPlayer_->GetVecPos());
        stunData_.vecKnockbackDirection_ = XMVector3Normalize(stunData_.vecKnockbackDirection_);
        if (boneData_.killTime_ < boneData_.killTimeMax_)
        {
            if (pBoneSuck_ != nullptr)
            {
                pBoneSuck_->SetKillTime(boneData_.killTimeWait_);
            }
            SetKillTime(boneData_.killTimeWait_);
        }
    }
}

void AttackPlayer::PlayerAddScore()
{
    PlayerBase::PlayerAddScore();
}

void AttackPlayer::PlayerCamera()
{
    PlayerBase::PlayerCamera();
    struct float2
    {
        float x, y;
    };

    struct XMMATRIX2
    {
        XMMATRIX x, y;
    };
    static float2 padRotMove = {};
    float2 sigmaRot = {};
    XMMATRIX2 mat2Rot = {};
    XMMATRIX matRot = {};
    float deadZone = 0.8f;
    XMVECTOR vecDir = {};
    XMFLOAT3 floDir_ = {};
    float floCameraLen = 30.0f;
    float floKnockbackLenRecedes = 5.0f;
    XMFLOAT3 mouseMove = Input::GetMouseMove();
    XMFLOAT3 padStickR = {};
    padStickR.x = Input::GetPadStickR(gameData_.padID_).x;
    padStickR.y = Input::GetPadStickR(gameData_.padID_).y;

    padRotMove.x = padStickR.x;
    padRotMove.y = -padStickR.y;
    if (Input::IsPadButton(XINPUT_GAMEPAD_DPAD_UP, gameData_.padID_))
    {
        if (moveData_.camZForwardFlag_ == false)
        {
            moveData_.camZForwardFlag_ = true;
            moveData_.CamPosZNum_ -= 1;
        }
    }
    else
    {
        moveData_.camZForwardFlag_ = false;
    }
    if (Input::IsPadButton(XINPUT_GAMEPAD_DPAD_DOWN, gameData_.padID_))
    {
        if (moveData_.camZBackFlag_ == false)
        {
            moveData_.camZBackFlag_ = true;
            moveData_.CamPosZNum_ += 1;
        }
    }
    else
    {
        moveData_.camZBackFlag_ = false;
    }
    if (moveData_.CamPosZNum_ <= 0)
    {
        moveData_.CamPosZNum_ = 0;
    }
    if (moveData_.CamPosZNum_ >= 4)
    {
        moveData_.CamPosZNum_ = 3;
    }
    moveData_.floLen_ = CamPosZ_[moveData_.CamPosZNum_];
    vecDir = vecFront;
    dirData_.vecCam_.x += padRotMove.y / padSensY;
    dirData_.vecCam_.y += padRotMove.x / padSensX;

    sigmaRot.y = dirData_.vecCam_.y;
    sigmaRot.x = -dirData_.vecCam_.x;

    if (sigmaRot.x > degreesMin * degreesToRadians)
    {
        sigmaRot.x = degreesMin;
        dirData_.vecCam_.x -= padRotMove.y / padSensX;
    }
    if (sigmaRot.x < degreesMax * degreesToRadians)
    {
        sigmaRot.x = degreesMax * degreesToRadians;
        dirData_.vecCam_.x -= padRotMove.y / padSensY;
    }

    mat2Rot.x = XMMatrixRotationX(sigmaRot.x);
    mat2Rot.y = XMMatrixRotationY(sigmaRot.y);

    matRot = mat2Rot.x * mat2Rot.y;
    vecDir = XMVector3Transform(vecDir, matRot);
    vecDir = XMVector3Normalize(vecDir);
    if (stunData_.isStun_)
    {
        static int floCameraLenPrev = floCameraLen;
        if (floCameraLen <= floCameraLenPrev + floKnockbackLenRecedes)
        {
            ++floCameraLen;
        }
    }
    else
    {
        static int floCameraLenPrev = floCameraLen;
        if (floCameraLen >= floCameraLenPrev - floKnockbackLenRecedes)
        {
            --floCameraLen;
        }
    }
    vecDir *= moveData_.floLen_ + floCameraLen;
    vecDir += XMLoadFloat3(&transform_.position_);
    XMStoreFloat3(&floDir_, vecDir);
    Camera::SetPosition(floDir_, gameData_.padID_);
    Camera::SetTarget(transform_.position_, gameData_.padID_);
}

void AttackPlayer::PlayerFall()
{
    PlayerBase::PlayerFall();
}

void AttackPlayer::PlayerMove()
{
    dirData_.vecDirection_ = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(gameData_.padID_);
    PlayerBase::PlayerMove();

    //▼プレイヤーの移動処理
    if (!moveData_.isRun_)
    {
        moveData_.padMoveSpeed_ = XMFLOAT3(walkSpeed, 0.0f, walkSpeed);
    }
    else
    {
        moveData_.padMoveSpeed_ = XMFLOAT3(runSpeed, 0.0f, runSpeed);
    }
    //▼向き変更
    XMFLOAT3 m;
    XMStoreFloat3(&m, dirData_.vecMove_);
    transform_.rotate_.y = XMConvertToDegrees(atan2(m.x, m.z));
    dirData_.angle_ = XMConvertToDegrees(atan2(m.x, m.z));

    //XMConvertToRadians = degree角をradian角に(ただ)変換する
    //XMMatrixRotationY = Y座標を中心に回転させる行列を作る関数

    dirData_.vecDirection_ = XMVectorSetY(dirData_.vecDirection_, 0);
    dirData_.vecDirection_ = XMVector3Normalize(dirData_.vecDirection_);


    moveData_.padMoveSpeed_.x *= XMVectorGetX(dirData_.vecDirection_);
    moveData_.padMoveSpeed_.z *= XMVectorGetZ(dirData_.vecDirection_);
    XMVECTOR tempvec = XMVector3Transform(dirData_.vecDirection_, rotmat);
    if (Input::GetPadStickL(gameData_.padID_).y > deadZone)   //前への移動
    {
        ApplyMovement(plusDir, plusDir);
    }
    if (Input::GetPadStickL(gameData_.padID_).y < -deadZone)  //後ろへの移動
    {
        ApplyMovement(minusDir, minusDir);
    }
    if (Input::GetPadStickL(gameData_.padID_).x > deadZone)   //右への移動
    {
        moveData_.padMoveSpeed_.x = 0.3f * XMVectorGetX(tempvec);
        moveData_.padMoveSpeed_.z = 0.3f * XMVectorGetZ(tempvec);
        ApplyMovement(plusDir, plusDir);
    }
    if (Input::GetPadStickL(gameData_.padID_).x < -deadZone)  //左への移動
    {
        moveData_.padMoveSpeed_.x = 0.3f * XMVectorGetX(tempvec);
        moveData_.padMoveSpeed_.z = 0.3f * XMVectorGetZ(tempvec);
        ApplyMovement(minusDir, minusDir);
    }

    if (!(Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER, gameData_.padID_)))
    {
        XMVECTOR vecCam = {};
        moveData_.CamPositionVec_ = Camera::VecGetPosition(gameData_.padID_);
        vecCam = -(moveData_.CamPositionVec_ - Camera::VecGetTarget(gameData_.padID_));
        XMFLOAT3 camRot = {};
        XMStoreFloat3(&camRot, vecCam);
        camRot.y = 0;
        vecCam = XMLoadFloat3(&camRot);
        vecCam = XMVector3Normalize(vecCam);
        dirData_.vecMove_ = vecCam;
    }

    if (Input::IsPadButton(XINPUT_GAMEPAD_A, gameData_.padID_) && !jumpData_.isJump_)
    {
        PlayerJumpPower();
        Audio::Stop(hSound_[((int)WALK)]);
        Audio::Stop(hSound_[((int)RUN)]);
        Audio::Play(hSound_[((int)JUMP)],soundData_.soundVolumeHalf_);
    }
    PlayerOuterWall();
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
        if (rayFloorDistUp == wallData_.distMax_)
        {
            Model::RayCast(floorHModel_ + i, &floorDataDown);  //レイを発射
        }
        wallData_.rayFloorDistDown_ = floorDataDown.dist;
        if (wallData_.rayFloorDistDown_ + jumpData_.positionY_ <= moveData_.isFling_)
        {
            if (!jumpData_.isJump_)
            {
                floorData_.isOnFloor_ = true;
                diveData_.isDived_ = false;
                jumpData_.positionY_ = -wallData_.rayFloorDistDown_ + jumpData_.playerInitPosY_;
                jumpData_.positionTempY_ = jumpData_.positionY_;
                jumpData_.positionPrevY_ = jumpData_.positionTempY_;
            }
        }
        else
        {
            floorData_.isOnFloor_ = false;
        }

    }
    //▼下の法線(地面に張り付き)
    stageDataDown.start = transform_.position_;   //レイの発射位置
    stageDataDown.start.y = initZeroFloat;
    XMStoreFloat3(&stageDataDown.dir, vecDown);   //レイの方向
    Model::RayCast(stageHModel_, &stageDataDown); //レイを発射
    wallData_.rayStageDistDown_ = stageDataDown.dist;
    //▼プレイヤーが浮いていないとき
    if (wallData_.rayStageDistDown_ + jumpData_.positionY_ <= moveData_.isFling_)
    {
        //▼ジャンプしてない＆すり抜け床の上にいない
        if (!jumpData_.isJump_ && !floorData_.isOnFloor_)
        {
            //▼地面に張り付き
            diveData_.isDived_ = false;
            jumpData_.positionY_ = -wallData_.rayStageDistDown_ + jumpData_.playerInitPosY_;
            jumpData_.positionTempY_ = jumpData_.positionY_;
            jumpData_.positionPrevY_ = jumpData_.positionTempY_;
        }
    }
    //▼前の法線(壁の当たり判定)
    stageDataFront.start = transform_.position_;      //レイの発射位置
    XMStoreFloat3(&stageDataFront.dir, vecFrontUp);   //レイの方向
    Model::RayCast(stageHModel_, &stageDataFront);    //レイを発射
    rayStageDistFront = stageDataFront.dist;
    if (rayStageDistFront <= wallData_.inTheWall_)
    {
        transform_.position_.z = moveData_.positionPrev_.z;
    }
    //▼後ろの法線(壁の当たり判定)
    stageDataBack.start = transform_.position_;       //レイの発射位置
    XMStoreFloat3(&stageDataBack.dir, vecBackUp);     //レイの方向
    Model::RayCast(stageHModel_, &stageDataBack);     //レイを発射
    rayStageDistBack = stageDataBack.dist;
    if (rayStageDistBack <= wallData_.inTheWall_)
    {
        transform_.position_.z = moveData_.positionPrev_.z;
    }
    //▼左の法線(壁の当たり判定)
    stageDataLeft.start = transform_.position_;       //レイの発射位置
    XMStoreFloat3(&stageDataLeft.dir, vecLeftUp);     //レイの方向
    Model::RayCast(stageHModel_, &stageDataLeft);     //レイを発射
    rayStageDistLeft = stageDataLeft.dist;
    if (rayStageDistLeft <= wallData_.inTheWall_)
    {
        transform_.position_.x = moveData_.positionPrev_.x;
    }
    //▼右の法線(壁の当たり判定)
    stageDataRight.start = transform_.position_;      //レイの発射位置
    XMStoreFloat3(&stageDataRight.dir, vecRightUp);   //レイの方向
    Model::RayCast(stageHModel_, &stageDataRight);    //レイを発射
    rayStageDistRight = stageDataRight.dist;
    if (rayStageDistRight <= wallData_.inTheWall_)
    {
        transform_.position_.x = moveData_.positionPrev_.x;
    }
    moveData_.positionPrev_ = transform_.position_;
}

void AttackPlayer::PlayerRevival()
{
    PlayerBase::PlayerRevival();
}

void AttackPlayer::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
    PlayerBase::SetKnockback(_vecKnockbackDirection, _knockbackSpeed);
}

void AttackPlayer::SetImageSecInit()
{
    pBoneImageManager_->SecInit();
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
    if (Input::GetPadTrrigerR(gameData_.padID_))
    {
        diveData_.isDive_ = true;
    }
}
