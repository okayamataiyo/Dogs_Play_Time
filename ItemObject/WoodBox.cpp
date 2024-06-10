//インクルード
#include "../Engine/Model.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/Fbx.h"
#include "../Engine/ImGui/imgui.h"
#include "../Engine/Audio.h"
#include "../Engine/Global.h"
#include "../StageObject/Stage.h"
#include "../Player/AttackPlayer.h"
#include "../Scene/Dogs_Walk_PlayScene.h"
#include "../Scene/Dogs_Fight_PlayScene.h"
#include "ItemObjectManager.h"
#include "WoodBox.h"

WoodBox::WoodBox(GameObject* _pParent)
    :ItemObjectBase(_pParent, woodBoxName), hModel_{ -1 }, hSound_{ -1 },soundVolume_{0.3f}
    , isBreak_{ false }, woodBoxs_{}, gravity_{ 0.007f },woodBoxInitposY_{1.2f}, positionY_{0.0f}, positionPrevY_{0.0f}, positionTempY_{0.0f}
    , isJump_{ false },isOnWoodBox_ {false}, rayWoodBoxDist_{ 0.0f }, rayStageDistDown_{ 0.0f }, isFling_{ 1.1f }
    ,pParent_{nullptr},pDogs_Walk_PlayScene_{nullptr},pDogs_Fight_PlayScene_{nullptr}, pAttackPlayer_{nullptr}, pCollision_{nullptr}
{
    pParent_ = _pParent;
}

WoodBox::~WoodBox()
{ 

}

void WoodBox::Initialize()
{
    //▼INIファイルからデータのロード
    walkOrFight_ = GetPrivateProfileInt("PLAYSCENEID", "WalkOrFight", 0, "Setting/PlaySceneSetting.ini");
    //サウンドデータのロード
    std::string soundName = soundFolderName + soundWoodBoxName + soundModifierName;
    hSound_ = Audio::Load(soundName);
    assert(hSound_ >= initZeroInt);
    //モデルデータのロード
    std::string modelName = modelFolderName + woodBoxName + modelModifierName;
    hModel_ = Model::Load(modelName);
    assert(hModel_ >= initZeroInt);
    XMFLOAT3 collisionPos = XMFLOAT3(0.0f,2.5f,0.0f);
    pCollision_ = new SphereCollider(collisionPos, 3.0f);
    AddCollider(pCollision_);
    pDogs_Walk_PlayScene_ = (Dogs_Walk_PlayScene*)FindObject(Dogs_Walk_PlaySceneName);
    pDogs_Fight_PlayScene_ = (Dogs_Fight_PlayScene*)FindObject(Dogs_Fight_PlaySceneName);
    pAttackPlayer_ = (AttackPlayer*)FindObject(attackPlayerName);
}

void WoodBox::Update()
{
    if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSWALK)
    {
        woodBoxs_ = pDogs_Walk_PlayScene_->GetWoodBoxs();
    }
    if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
    {
    	woodBoxs_ = pDogs_Fight_PlayScene_->GetWoodBoxs();
    }
    WoodBoxMove();
    WoodBoxFall();
    WoodBoxRayCast();
    transform_.position_.y = positionY_;
    if (isBreak_)
    {
        WoodBoxDeath();
    }
}

void WoodBox::BothViewDraw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void WoodBox::LeftViewDraw()
{
}

void WoodBox::RightViewDraw()
{
}

void WoodBox::UPSubViewDraw()
{
}

void WoodBox::Release()
{
}

void WoodBox::WoodBoxFall()
{
    if (isJump_)
    {
        //放物線に下がる処理
        positionTempY_ = positionY_;
        positionY_ += (positionY_ - positionPrevY_) - gravity_;
        positionPrevY_ = positionTempY_;
        if (isFling_ >= rayStageDistDown_)
        {
            isJump_ = false;
        }
    }
}

void WoodBox::WoodBoxMove()
{
}

void WoodBox::WoodBoxRayCast()
{
    RayCastData woodBoxDataDown;
    RayCastData stageDataDown;
    int woodBoxHModelStart = woodBoxs_.front();
    int woodBoxHModelEnd = woodBoxs_.back();
    int woodBoxHModelNow = GetModelHandle();
    Stage* pStage = (Stage*)FindObject(stageName);      //ステージオブジェクト
    int stageHModel = pStage->GetModelHandle();         //モデル番号を取得

    //▼ステージの法線(地面に張り付き)
    stageDataDown.start = transform_.position_;         //レイの発射位置
    XMStoreFloat3(&stageDataDown.dir, vecDown);         //レイの方向
    Model::RayCast(stageHModel, &stageDataDown);        //レイを発射
    rayStageDistDown_ = stageDataDown.dist;
    if (rayStageDistDown_ <= isFling_)
    {
        if (!isJump_)
        {
            //positionY_ += woodBoxInitposY_;
        }
    }
    if (rayStageDistDown_ == 99999.0f)
    {
        rayStageDistDown_ = 0;
    }
    if(isFling_ <= rayStageDistDown_ && !isOnWoodBox_)// && stageDataDown.hit)
    {
        isJump_ = true;
    }
}

void WoodBox::WoodBoxDeath()
{
    Audio::Play(hSound_, soundVolume_);
    this->KillMe();
}

void WoodBox::OnCollision(GameObject* _pTarget)
{

}
