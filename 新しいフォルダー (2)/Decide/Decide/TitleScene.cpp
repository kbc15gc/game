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
	//スタートボタンの押下確認
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START);
	//エンターキー
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)) && !_ChangeScene)
	{
		//シーンチェンジフラグ
		_ChangeScene = true;
		SetFade(true);
	}
	if (_ChangeScene &&	//エンターキーが押された
		!_IsFade)		//フェード終了
	{
		//ゲームシーンへ移行
		INSTANCE(SceneManager)->ChangeScene("GameScene");
		_ChangeScene = false;
		return;
	}
}
