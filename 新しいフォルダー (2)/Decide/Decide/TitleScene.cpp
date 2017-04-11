#include "TitleScene.h",
#include"fbEngine/ImageObject.h"
#include "GameCamera.h"
#include "GameLight.h"

void TitleScene::Start()
{
	ImageObject* test = GameObjectManager::AddNew<ImageObject>("title",0);
	test->SetTexture(LOADTEXTURE("title.png"));
	test->SetPivot(0.0f, 0.0f);
}

void TitleScene::Update()
{
	//�X�^�[�g�{�^���̉����m�F
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START);
	//�G���^�[�L�[
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)) && !_ChangeScene)
	{
		//�V�[���`�F���W�t���O
		_ChangeScene = true;
		SetFade(true);
	}
	if (_ChangeScene &&	//�G���^�[�L�[�������ꂽ
		!_IsFade)		//�t�F�[�h�I��
	{
		//�Q�[���V�[���ֈڍs
		INSTANCE(SceneManager)->ChangeScene("GameScene");
		_ChangeScene = false;
		return;
	}
}
