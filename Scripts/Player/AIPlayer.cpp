//インクルード
#include <random>
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
#include "../State/AIPlayerState.h"
#include "../Scene/Dogs_Walk_PlayScene.h"
#include "../Scene/Dogs_Fight_PlayScene.h"
#include "../ImageManager.h"
#include "../ParticleManager.h"
#include "../UIManager.h"
#include "../BehaviourTree/AIPlayerWaitSelector.h"
#include "AIPlayer.h"
#include "CollectPlayer.h"
#include "AttackPlayer.h"

using enum AIPlayer::SOUNDSTATE;
using enum ImageManager::IMAGESTATE;
using enum PlayerBase::GAMESTATE;
using enum PLAYSCENESTATE;
using enum Node::NODESTATE;

AIPlayer::AIPlayer(GameObject* _pParent)
    :PlayerBase(_pParent, aIPlayerName)
    ,pParent_{_pParent}, hModel_{-1}, hSound_{-1,-1,-1,-1}, stageHModel_{-1}, floorHModel_{-1}
    , number_{ 0 }, gameState_{ GAMEREADY },waitTime_{ 60 },waitTimeWait_{60}, attackTime_{90}, attackTimeWait_{90}
    ,attackSeeTime_{60},attackSeeTimeWait_{60}, isAttack_{false},isAttackFinish_{false}, isAttackSee_{false}, isAttackSeeFinish_{false}
    , slowTime_{ 0 }, slowTimeWait_{ 1 }, dir_{}, random_value_{0}
    ,pDogs_Walk_PlayScene_{ nullptr }, pDogs_Fight_PlayScene_{ nullptr }, pCollectPlayer_{ nullptr }, pCollision_{ nullptr }
    ,pAttackPlayer_{nullptr}, pWoodBox_{nullptr}, pStage_{nullptr}, pFloor_{nullptr}
    , pSceneManager_{ nullptr }, pItemObjectManager_{ nullptr }, pStateManager_{ nullptr }, pImageManager_{nullptr}
    , pBoneImageManager_{ nullptr }, pParticleManager_{ nullptr },pAIPlayerWaitSelector_{nullptr}
{
}

AIPlayer::~AIPlayer()
{
}

void AIPlayer::Initialize()
{
    //▼INIファイルからデータのロード
    gameData_.walkOrFight_ = GetPrivateProfileInt("PLAYSCENEID", "WalkOrFight", 0, "Setting/PlaySceneSetting.ini");
    //▼サウンドデータのロード
    std::string soundName;
    for (int i = 0; i < sizeof(soundAIPlayerNames) / sizeof(soundAIPlayerNames[initZeroInt]); i++)
    {
        soundName = soundFolderName + soundAIPlayerNames[i] + soundModifierName;
        hSound_[i] = Audio::Load(soundName);
        assert(hSound_[i] >= 0);
    }
    //▼モデルデータのロード
    std::string modelName = modelFolderName + aIPlayerName + modelModifierName;
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
    pStateManager_->AddState("WalkState", new AIPlayerWalkState(pStateManager_));
    pStateManager_->AddState("WaitState", new AIPlayerWaitState(pStateManager_));
    pStateManager_->AddState("RunState", new AIPlayerRunState(pStateManager_));
    pStateManager_->AddState("JumpState", new AIPlayerJumpState(pStateManager_));
    pStateManager_->AddState("StunState", new AIPlayerStunState(pStateManager_));
    pStateManager_->ChangeState("WaitState");
    dirData_.vecDirection_ = dir_;
    pParticleManager_ = Instantiate<ParticleManager>(this);
    pImageManager_ = Instantiate<ImageManager>(this);
    pImageManager_->SetMode((int)NONEIMAGE);
    pImageManager_->SecInit();
    pBoneImageManager_ = Instantiate<ImageManager>(this);
    pBoneImageManager_->SetMode((int)BONEIMAGE);
    pAIPlayerWaitSelector_ = new AIPlayerWaitSelector(nullptr, this);
    diveData_.divePower_ = 0.4f;
}

void AIPlayer::Update()
{
    //▼ステートマネージャーの更新
    pStateManager_->Update();

    switch (gameState_)
    {
    case GAMEREADY:          UpdateReady();      break;
    case GAMEPLAY:           UpdatePlay();       break;
    case GAMEOVER:       UpdateGameOver();   break;
    }
}

void AIPlayer::UpdateSlow()
{
    PlayerKnockback();
}

void AIPlayer::BothViewDraw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void AIPlayer::LeftViewDraw()
{

}

void AIPlayer::RightViewDraw()
{

}

void AIPlayer::UPSubViewDraw()
{

}

void AIPlayer::Release()
{
    SAFE_DELETE(pStateManager_);
}

void AIPlayer::UpdateReady()
{
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

void AIPlayer::UpdatePlay()
{
    PlayerFall();
    PlayerRayCast();
    pAIPlayerWaitSelector_->ChoiceUpdate();
    if (pAIPlayerWaitSelector_->GetMyNodeState() == READY)
    {
        pAIPlayerWaitSelector_->SetMyNodeState(RUNNING);
    }

    --waitTime_;
    if (waitTime_ <= 0)
    {
        isAttackSee_ = true;
    }

    //▼落ちた時の処理
    if (transform_.position_.y <= -gameData_.fallLimit_)
    {
        PlayerRevival();
        PlayerStun(revivalTime_);

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
    transform_.position_.y = jumpData_.positionY_;
    if (stunData_.isStun_)
    {
        slowTimeWait_ = slowTimeNum_;
        stunData_.stunTimeCounter_++;
        if (stunData_.stunTimeCounter_ >= stunData_.stunLimit_)
        {
            slowTimeWait_ = slowTimeNum_;
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
    if (moveData_.isMove_ && !jumpData_.isJump_ && !moveData_.isRun_)
    {
        Audio::Play(hSound_[((int)WALK)], soundData_.soundVolume_);
    }
    if (!moveData_.isMove_ && !jumpData_.isJump_)
    {
        Audio::Stop(hSound_[((int)WALK)]);
        Audio::Stop(hSound_[((int)RUN)]);
    }
    IsMove();
    IsJump();
    IsRun();
    IsStun();
    IsDive();
}

void AIPlayer::UpdateGameOver()
{
}

void AIPlayer::PlayerAttackActionFunc()
{
    isAttackSeeFinish_ = false;
    --attackTime_;
    if (attackTime_ <= 0)
    {
        waitTime_ = waitTimeWait_;
        attackTime_ = attackTimeWait_;
        isAttack_ = false;
        isAttackSee_ = false;
        isAttackFinish_ = true;
    }
    transform_.position_.x += 0.3f * XMVectorGetX(-dir_);
    transform_.position_.z += 0.3f * XMVectorGetZ(-dir_);
}

void AIPlayer::PlayerAttackSeeActionFunc()
{
    isAttackFinish_ = false;
    --attackSeeTime_;
    if(attackSeeTime_ <= 0)
    {
        attackSeeTime_ = attackSeeTimeWait_;
        isAttackSeeFinish_ = true;
        isAttack_ = true;
    }

    if (waitTime_ >= 0)
    {
        //乱数生成器の設定
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 2);

        //1から2までのランダムな値の作成
        random_value_ = dis(gen);
    }

    //向き変更
    if (random_value_ == 1)
    {
        dir_ = (XMLoadFloat3(&transform_.position_) - pAttackPlayer_->GetVecPos());
    }
    if (random_value_ == 2)
    {
        dir_ = (XMLoadFloat3(&transform_.position_) - pCollectPlayer_->GetVecPos());
    }
    dir_ = XMVector3Normalize(dir_);
    XMFLOAT3 m;
    XMStoreFloat3(&m, -dir_);
    transform_.rotate_.y = XMConvertToDegrees(atan2(m.x, m.z));
    dirData_.angle_ = XMConvertToDegrees(atan2(m.x, m.z));
}

void AIPlayer::PlayerAttackDiveActionFunc()
{
    diveData_.isDive_ = true;
}

void AIPlayer::PlayerWaitStateFunc()
{
    PlayerBase::PlayerWaitStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AIPlayer::PlayerWalkStateFunc()
{
    PlayerBase::PlayerWalkStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AIPlayer::PlayerRunStateFunc()
{
    PlayerBase::PlayerRunStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AIPlayer::PlayerJumpStateFunc()
{
    PlayerBase::PlayerJumpStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AIPlayer::PlayerStunStateFunc()
{
    PlayerBase::PlayerStunStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AIPlayer::PlayerStun(int _timeLimit)
{
    PlayerBase::PlayerStun(_timeLimit);
}

void AIPlayer::PlayerOuterWall()
{
    PlayerBase::PlayerOuterWall();
}

void AIPlayer::OnCollision(GameObject* _pTarget)
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
                pDogs_Walk_PlayScene_->AddWoodBoxCount(-woodBoxData_.woodBoxDeath_);
            }
            if (gameData_.walkOrFight_ == (int)DOGSFIGHT)
            {
                pDogs_Fight_PlayScene_->AddWoodBoxCount(-woodBoxData_.woodBoxDeath_);
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
            SetKillTime(boneData_.killTimeWait_);
        }
    }

    if (_pTarget->GetObjectName() == attackPlayerName)
    {
        PlayerStun(stunData_.stunTime_);
        stunData_.isKnockBack_ = true;
        stunData_.vecKnockbackDirection_ = (XMLoadFloat3(&transform_.position_) - pAttackPlayer_->GetVecPos());
        stunData_.vecKnockbackDirection_ = XMVector3Normalize(stunData_.vecKnockbackDirection_);
        if (boneData_.killTime_ < boneData_.killTimeMax_)
        {
            SetKillTime(boneData_.killTimeWait_);
        }
    }
}

void AIPlayer::PlayerAddScore()
{
    PlayerBase::PlayerAddScore();
}

void AIPlayer::PlayerCamera()
{
    PlayerBase::PlayerCamera();
}

void AIPlayer::PlayerFall()
{
    PlayerBase::PlayerFall();
}

void AIPlayer::PlayerMove()
{
    dirData_.vecDirection_ = -dir_;
    PlayerOuterWall();
    PlayerBase::PlayerMove();
}

void AIPlayer::PlayerJump()
{

}

void AIPlayer::PlayerJumpPower()
{
    PlayerBase::PlayerJumpPower();
}

void AIPlayer::PlayerDive()
{
    PlayerBase::PlayerDive();
}

void AIPlayer::PlayerDivePower()
{
    PlayerBase::PlayerDivePower();
}

void AIPlayer::PlayerKnockback()
{
    PlayerBase::PlayerKnockback();
}

void AIPlayer::PlayerRayCast()
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

void AIPlayer::PlayerRevival()
{
    PlayerBase::PlayerRevival();
}

void AIPlayer::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
    PlayerBase::SetKnockback(_vecKnockbackDirection, _knockbackSpeed);
}

void AIPlayer::SetImageSecInit()
{
    pBoneImageManager_->SecInit();
}

void AIPlayer::IsMove()
{
    PlayerBase::IsMove();
}

void AIPlayer::IsJump()
{
    PlayerBase::IsJump();
}

void AIPlayer::IsRun()
{
    PlayerBase::IsRun();
}

void AIPlayer::IsStun()
{
    PlayerBase::IsStun();
}

void AIPlayer::IsDive()
{
    PlayerBase::IsDive();
}
