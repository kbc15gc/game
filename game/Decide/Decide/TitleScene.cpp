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
	//スタートボタンの押下確認
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START);
	//エンターキー
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)))
	{
		//フェードイン開始
		StartFade(true);
	}
	//フェードイン完了
	if (_FadeState == fbScene::FadeStateE::INEND)
	{
		//ゲームシーンへ移行
		INSTANCE(SceneManager)->ChangeScene("GameScene");
		return;
	}
}
