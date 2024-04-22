#include "sceneManager.h"

#include "../Scene/PlayScene.h"
#include "../Scene/GameOverScene.h"
#include "../Scene/SelectScene.h"
#include "../Scene/GameTitleScene.h"
#include "Model.h"
#include "Image.h"
#include "Audio.h"


//�R���X�g���N�^
SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, sceneManagerName)
{
}

//������
void SceneManager::Initialize()
{
	//�ŏ��̃V�[��������
	currentSceneID_ = SCENE_ID_GAMETITLE;
	nextSceneID_ = currentSceneID_;
	Instantiate<GameTitleScene>(this);
}

//�X�V
void SceneManager::Update()
{
	//���̃V�[�������݂̃V�[���ƈႤ�@���@�V�[����؂�ւ��Ȃ���΂Ȃ�Ȃ�
	if (currentSceneID_ != nextSceneID_)
	{
		//���̃V�[���̃I�u�W�F�N�g��S�폜
		KillAllChildren();

		//���[�h�����f�[�^��S�폜
		Audio::Release();
		Model::AllRelease();
		Image::AllRelease();

		//���̃V�[�����쐬
		switch (nextSceneID_)
		{
		case SCENE_ID_PLAY: Instantiate<PlayScene>(this); break;
		case SCENE_ID_GAMEOVER:Instantiate<GameOverScene>(this); break;
		case SCENE_ID_SELECT: Instantiate<SelectScene>(this); break;
		case SCENE_ID_GAMETITLE: Instantiate<GameTitleScene>(this); break;

		}
		Audio::Initialize();
		currentSceneID_ = nextSceneID_;
	}
}

//�`��
void SceneManager::Draw()
{
}

//�J��
void SceneManager::Release()
{
}

//�V�[���؂�ւ��i���ۂɐ؂�ւ��̂͂��̎��̃t���[���j
void SceneManager::ChangeScene(SCENE_ID next)
{
	nextSceneID_ = next;
}