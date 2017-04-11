#include "GameScene.h"
#include "GameCamera.h"
#include "GameLight.h"
#include "GameShadowCamera.h"
#include "Player.h"
#include "Ground.h"
#include "Sky.h"
#include "fbEngine/ImageObject.h"
#include "fbEngine/TextObject.h"
#include "fbEngine/Sprite.h"
#include "fbEngine/SoundSource.h"

void GameScene::Start()
{
	GameObjectManager::AddNew<GameCamera>("GameCamera", 2);
	GameObjectManager::AddNew<GameLight>("GameLight", 0);
	GameObjectManager::AddNew<GameShadowCamera>("GameShadowCamera", 0);
	//�v���C���[����
	GameObjectManager::AddNew<Player>("Player", 1);

	//�n�ʐ���
	GameObjectManager::AddNew<Ground>("Ground", 1);
	//�󐶐�
	GameObjectManager::AddNew<Sky>("Sky", 1);
}

void GameScene::Update()
{
	//�X�^�[�g�{�^���̉����m�F
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START);
	//�G���^�[�L�[
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)) && !_ChangeScene)
	{
		//�`�F���W�V�[���t���O��true
		_ChangeScene = true;
		//�t�F�[�h�J�n
		SetFade(true);
	}
	if (_ChangeScene &&	//�G���^�[�L�[�������ꂽ
		!_IsFade)		//�t�F�[�h�I��
	{
		//�^�C�g���V�[���ֈڍs
		INSTANCE(SceneManager)->ChangeScene("TitleScene");
		//�V�[���`�F���W����
		_ChangeScene = false;
		return;
	}
}