#pragma once
#include "global.h"
#include "GameObject.h"

//�Q�[���ɓo�ꂷ��V�[��
enum SCENE_ID
{
	SCENE_ID_DOGS_WALK_PLAY = 0,
	SCENE_ID_DOGS_FIGHT_PLAY,
	SCENE_ID_GAMEOVER,
	SCENE_ID_SELECT,
	SCENE_ID_GAMESELECT,
	SCENE_ID_GAMETITLE,
};

namespace
{
	std::string sceneManagerName = "SceneManager";
}

//-----------------------------------------------------------
//�V�[���؂�ւ���S������I�u�W�F�N�g
//-----------------------------------------------------------
class SceneManager : public GameObject
{
private:
	SCENE_ID currentSceneID_;	//���݂̃V�[��
	SCENE_ID nextSceneID_;		//���̃V�[��
public:

	//�R���X�g���N�^
	//�����Fparent	�e�I�u�W�F�N�g�i��{�I�ɃQ�[���}�l�[�W���[�j
	SceneManager(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void BothViewDraw() override;
	void LeftViewDraw() override;
	void RightViewDraw() override;
	void UPSubViewDraw() override;
	void Release() override;

	//�V�[���؂�ւ��i���ۂɐ؂�ւ��̂͂��̎��̃t���[���j
	//�����Fnext	���̃V�[����ID
	void ChangeScene(SCENE_ID next);
};