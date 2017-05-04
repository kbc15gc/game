#include "TitleScene.h"
#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "GameCamera.h"
#include "GameLight.h"

void TitleScene::Start()
{
	ImageObject* title = INSTANCE(GameObjectManager)->AddNew<ImageObject>("title",0);
	title->SetTexture(LOADTEXTURE("title.png"));
	title->SetPivot(0.0f, 0.0f);
}

void TitleScene::Update()
{
	//�X�^�[�g�{�^���̉����m�F
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START);
	//�G���^�[�L�[
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)))
	{
		//�t�F�[�h�C���J�n
		StartFade(true);
	}
	//�t�F�[�h�C������
	if (_FadeState == fbScene::FadeStateE::INEND)
	{
		//�Q�[���V�[���ֈڍs
		INSTANCE(SceneManager)->ChangeScene("GameScene");
		return;
	}
}
