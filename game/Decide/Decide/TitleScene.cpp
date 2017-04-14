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
	//スタートボタンの押下確認
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START);
	//エンターキー
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)) && !_ChangeScene)
	{
		//シーンチェンジフラグtrue
		_ChangeScene = true;
		//フェード開始
		SetFade(true);
	}
	if (_ChangeScene &&	//エンターキーが押された
		!_IsFade)		//フェード終了
	{
		//ゲームシーンへ移行
		INSTANCE(SceneManager)->ChangeScene("GameScene");
		//シーンチェンジ完了
		_ChangeScene = false;
		return;
	}
}
