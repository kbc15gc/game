#include "TitleScene.h"
#include "fbEngine/ImageObject.h"
#include "GameCamera.h"
#include "GameLight.h"

void TitleScene::Start()
{
	ImageObject* title = GameObjectManager::AddNew<ImageObject>("title",0);
	title->SetTexture(LOADTEXTURE("title.png"));
	title->SetPivot(0.0f, 0.0f);
}

void TitleScene::Update()
{
	//�X�^�[�g�{�^���̉����m�F
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START);
	//�G���^�[�L�[
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)) && !_ChangeScene)
	{
		//�V�[���`�F���W�t���Otrue
		_ChangeScene = true;
		//�t�F�[�h�J�n
		SetFade(true);
	}
	if (_ChangeScene &&	//�G���^�[�L�[�������ꂽ
		!_IsFade)		//�t�F�[�h�I��
	{
		//�Q�[���V�[���ֈڍs
		INSTANCE(SceneManager)->ChangeScene("GameScene");
		//�V�[���`�F���W����
		_ChangeScene = false;
		return;
	}
}
