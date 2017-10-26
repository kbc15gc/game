#include"stdafx.h"
#include "TitleScene.h"
#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "GameObject\Camera\GameCamera.h"
#include "GameLight.h"

void TitleScene::Start()
{
	ImageObject* title = INSTANCE(GameObjectManager)->AddNew<ImageObject>("title",0);
	title->SetTexture(LOADTEXTURE("Title.png"));
	title->SetPivot(0.0f, 0.0f);
	title->SetSize({ WindowW, WindowH });

	_Start = INSTANCE(GameObjectManager)->AddNew<ImageObject>("Start", 0);
	_Start->SetTexture(LOADTEXTURE("StartButton.png"));
	_Start->SetPivot(0.0f, 0.0f);
	_Start->SetSize({ WindowW, WindowH });
	_StartColor = Color::white;
	_StartAlpha = 0.01f;

	_StartSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
	_StartSE->Init("Asset/Sound/start.wav");
	_StartSE->SetVolume(1.5f);

	_TitleBGM = INSTANCE(GameObjectManager)->AddNew<SoundSource>("TitleBGM", 0);
	_TitleBGM->InitStreaming("Asset/Sound/titleBgm.WAV");
	_TitleBGM->Play(true);

	INSTANCE(SceneManager)->GetSky()->SetActive(false);
}

void TitleScene::Update()
{
	//スタートボタンの押下確認
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_A) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_B) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_X) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_Y);
	//エンターキー
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)))
	{
		////ゲームシーンへ移行
		INSTANCE(SceneManager)->ChangeScene("GameScene",true);
	
		// テスト。
		//INSTANCE(SceneManager)->ChangeScene("LastBossTestScene", true);

		_StartSE->Play(false);
		return;
	}

	//点滅させる。
	Alpha();
}

void TitleScene::Alpha()
{
	_StartColor.a -= _StartAlpha;
	_Start->SetBlendColor(_StartColor);
	if (_StartColor.a <= 0.0f || _StartColor.a >= 1.0f)
	{
		_StartAlpha *= -1;
	}
}
