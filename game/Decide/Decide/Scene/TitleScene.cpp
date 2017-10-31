#include"stdafx.h"
#include "TitleScene.h"
#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "GameObject\Camera\GameCamera.h"
#include "GameLight.h"

void TitleScene::Start()
{
	//タイトル画像
	ImageObject* title = INSTANCE(GameObjectManager)->AddNew<ImageObject>("title",0);
	title->SetTexture(LOADTEXTURE("Title.png"));
	title->SetPivot(0.0f, 0.0f);
	title->SetSize({ WindowW, WindowH });

	//PressAnyButton画像
	_Start = INSTANCE(GameObjectManager)->AddNew<ImageObject>("Start", 0);
	_Start->SetTexture(LOADTEXTURE("StartButton.png"));
	_Start->SetPivot(0.0f, 0.0f);
	_Start->SetSize({ WindowW, WindowH });

	//カラー
	_StartColor = Color::white;
	_StartColor.a = 0.3f;
	_StartAlpha = 0.0025f;

	//ボタンバー
	_StartBar = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StartBar", 0);
	_StartBar->SetTexture(LOADTEXTURE("BAR.png"));
	_StartBar->SetPivot(0.0f, 0.0f);
	_StartBar->SetSize({ 400, 85 });
	_StartBar->SetBlendColor(_StartColor);
	_StartBar->transform->SetPosition(455,440,0);

	//セレクトボタン
	_SelectButton = INSTANCE(GameObjectManager)->AddNew<ImageObject>("SELECTBUTTON", 0);
	_SelectButton->SetTexture(LOADTEXTURE("SELECTBUTTON.png"));
	_SelectButton->SetPivot(0.0f, 0.0f);
	_SelectButton->SetSize({ WindowW, WindowH });
	_SelectButton->SetActive(false);

	//ボタン音
	_StartSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
	_StartSE->Init("Asset/Sound/start.wav");
	_StartSE->SetVolume(1.5f);

	//タイトルBGM
	_TitleBGM = INSTANCE(GameObjectManager)->AddNew<SoundSource>("TitleBGM", 0);
	_TitleBGM->InitStreaming("Asset/Sound/titleBgm.WAV");
	_TitleBGM->Play(true);

	//ボタンのフラグ
	_AnyButton = false;

	//セレクト
	_Select = Select::Continue;

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

	//bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_A) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_B) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_X) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_Y);

	//プレスエニイボタンの処理
	if (!_AnyButton && flag)
	{
		_Start->SetActive(false);
		_SelectButton->SetActive(true);
		_StartBar->transform->SetPosition(455, 400, 0);
		_AnyButton = true;
		_StartSE->Play(false);
		flag = false;
	}

	//コンティニューかニューゲームの処理
	if (_AnyButton)
	{
		
		if (INSTANCE(InputManager)->GetXInput(0)->IsPressAnalog(AnalogE::L_STICKU))
		{
			_Select = Select::Continue;
			_StartBar->transform->SetPosition(455, 400, 0);
		}
		if (INSTANCE(InputManager)->GetXInput(0)->IsPressAnalog(AnalogE::L_STICKD))
		{
			_Select = Select::NewGame;
			_StartBar->transform->SetPosition(455, 470, 0);
		}

		//スタート
		if (flag)
		{
			//ゲームシーンへ移行
			_StartSE->Play(false);
			INSTANCE(SceneManager)->ChangeScene("GameScene",true);
		}
	}

	//点滅させる。
	Alpha();
}

void TitleScene::Alpha()
{
	_StartColor.a -= _StartAlpha;
	//_Start->SetBlendColor(_StartColor);
	_StartBar->SetBlendColor(_StartColor);
	if (_StartColor.a <= 0.2f || _StartColor.a >= 0.5f)
	{
		_StartAlpha *= -1;
	}
}
