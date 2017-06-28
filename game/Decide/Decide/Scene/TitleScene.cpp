#include"stdafx.h"
#include "TitleScene.h"
#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "GameObject\Camera\GameCamera.h"
#include "GameLight.h"

void TitleScene::Start()
{
	ImageObject* title = INSTANCE(GameObjectManager)->AddNew<ImageObject>("title",0);
	title->SetTexture(LOADTEXTURE("title.png"));
	title->SetPivot(0.0f, 0.0f);

	_TitleBGM = INSTANCE(GameObjectManager)->AddNew<SoundSource>("TitleBGM", 0);
	_TitleBGM->InitStreaming("Asset/Sound/titleBgm.WAV");
	_TitleBGM->Play(true);

	INSTANCE(SceneManager)->GetSky()->SetActive(false);
}

void TitleScene::Update()
{
	//スタートボタンの押下確認
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START);
	//エンターキー
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)))
	{
		//ゲームシーンへ移行
		INSTANCE(SceneManager)->ChangeScene("GameScene",true);
		return;
	}
}
