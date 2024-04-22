//インクルード
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Image.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/SolidText.h"
#include "../StageObject/Sky.h"
#include "../Player/AttackPlayer.h"
#include "../Player/CollectPlayer.h"
#include "SelectScene.h"

SelectScene::SelectScene(GameObject* _pParent)
	:GameObject(_pParent, selectSceneName), hPict_{ -1 }, solidTextRotate_{ 0.3f }, isViewPicture_{ false }, skyPos_{ 0.0f,0.0f,0.0f },skyPosFly_{10000.0f,0.0f,10000.0f}
	,pSceneManager_{nullptr}, pStageObjectManager_{nullptr},pSky_{nullptr}
{

}

void SelectScene::Initialize()
{
	//▼画像データのロード
	std::string imageName = modelFolderName + manualName + imageModifierName;
	hPict_ = Image::Load(imageName);
	assert(hPict_ >= initZeroInt);
	pictureTrans_ = {};
	ShowCursor();
	pSolidText_ = Instantiate<SolidText>(this);
	pSolidText_->SetMode(1);
	pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::SKY);
	pSky_ = (Sky*)pStageObjectManager_->GetStageObjectBase();
	camPos_ = pSolidText_->GetPosition();
	camPos_.y += 2;
	camPos_.z -= 15;
}

void SelectScene::Update()
{
	Camera::SetPosition(camPos_, attackPlayerNumber);
	Camera::SetTarget(pSolidText_->GetPosition(), attackPlayerNumber);
	Camera::SetPosition(camPos_, collectPlayerNumber);
	Camera::SetTarget(pSolidText_->GetPosition(), collectPlayerNumber);

	transform_.rotate_.y += solidTextRotate_;

	XMFLOAT3 pos = Input::GetMousePosition();
	if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, attackPlayerNumber) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, collectPlayerNumber))
	{
		if (Direct3D::GetIsChangeView() == (int)Direct3D::VIEWSTATE::LEFTVIEW)
		{
			Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::LEFT_BOTHVIEW);
		}
		if (Direct3D::GetIsChangeView() == (int)Direct3D::VIEWSTATE::RIGHTVIEW)
		{
			Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::RIGHT_BOTHVIEW);
		}
		pSceneManager_->ChangeScene(SCENE_ID_PLAY);
	}
	if (Input::IsKeyDown(DIK_R))
	{
		Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::RIGHT_BOTHVIEW);
	}
	if (Input::IsKeyDown(DIK_T))
	{
		Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::RIGHTVIEW);
	}
	if (Input::IsKeyDown(DIK_Y))
	{
		Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::LEFT_BOTHVIEW);
	}
	if (Input::IsKeyDown(DIK_U))
	{
		Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::LEFTVIEW);
	}

	if (Input::IsMouseButtonDown((int)MOUSESTATE::RIGHTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_X, attackPlayerNumber) || Input::IsPadButtonDown(XINPUT_GAMEPAD_X, collectPlayerNumber))
	{
		isViewPicture_ = !isViewPicture_;
	}
	if (isViewPicture_)
	{
		pSky_->SetPosition(skyPosFly_);
		pSolidText_->SetPosition(skyPosFly_);
	}
	else
	{
		pSky_->SetPosition(skyPos_);
		pSolidText_->SetPosition(skyPos_);
	}
}

void SelectScene::Draw()
{
	Image::SetTransform(hPict_, pictureTrans_);
	Image::Draw(hPict_);
}

void SelectScene::Release()
{
}
