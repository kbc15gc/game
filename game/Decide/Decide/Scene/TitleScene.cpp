#include"stdafx.h"
#include "TitleScene.h"
#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "GameObject\Camera\GameCamera.h"
#include "GameLight.h"

void TitleScene::Start()
{
	//�^�C�g���摜
	ImageObject* title = INSTANCE(GameObjectManager)->AddNew<ImageObject>("title",0);
	title->SetTexture(LOADTEXTURE("Title.png"));
	title->SetPivot(0.0f, 0.0f);
	title->SetSize({ WindowW, WindowH });

	//PressAnyButton�摜
	_Start = INSTANCE(GameObjectManager)->AddNew<ImageObject>("Start", 0);
	_Start->SetTexture(LOADTEXTURE("StartButton.png"));
	_Start->SetPivot(0.0f, 0.0f);
	_Start->SetSize({ WindowW, WindowH });

	//�J���[
	_StartColor = Color::white;
	_StartColor.a = 0.3f;
	_StartAlpha = 0.0025f;

	//�{�^���o�[
	_StartBar = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StartBar", 0);
	_StartBar->SetTexture(LOADTEXTURE("BAR.png"));
	_StartBar->SetPivot(0.0f, 0.0f);
	_StartBar->SetSize({ 400, 85 });
	_StartBar->SetBlendColor(_StartColor);
	_StartBar->transform->SetPosition(455,440,0);

	//�Z���N�g�{�^��
	//�j���[�Q�[��
	_NewGame = INSTANCE(GameObjectManager)->AddNew<ImageObject>("NEWGAME", 0);
	_NewGame->SetTexture(LOADTEXTURE("NEWGAME.png"));
	_NewGame->SetPivot(0.0f, 0.0f);
	_NewGame->SetActive(false);

	//�R���e�B�j���[
	_Continue = INSTANCE(GameObjectManager)->AddNew<ImageObject>("CONTINUE", 0);
	_Continue->SetTexture(LOADTEXTURE("CONTINUE.png"));
	_Continue->SetPivot(0.0f, 0.0f);
	_Continue->SetActive(false);

	//�{�^����
	_StartSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
	_StartSE->Init("Asset/Sound/start.wav");
	_StartSE->SetVolume(1.5f);

	//�^�C�g��BGM
	_TitleBGM = INSTANCE(GameObjectManager)->AddNew<SoundSource>("TitleBGM", 0);
	_TitleBGM->InitStreaming("Asset/Sound/titleBgm.WAV");
	_TitleBGM->Play(true);

	//�{�^���̃t���O
	_AnyButton = false;

	//�Z���N�g
	_Select = Select::NewGame;

	INSTANCE(SceneManager)->GetSky()->SetActive(false);
}

void TitleScene::Update()
{
	////�X�^�[�g�{�^���̉����m�F
	//bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_A) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_B) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_X) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_Y);
	////�G���^�[�L�[
	//if ((flag || KeyBoardInput->isPush(DIK_RETURN)))
	//{
	//	////�Q�[���V�[���ֈڍs
	//	INSTANCE(SceneManager)->ChangeScene("GameScene",true);
	//
	//	// �e�X�g�B
	//	//INSTANCE(SceneManager)->ChangeScene("LastBossTestScene", true);

	//	_StartSE->Play(false);
	//	return;
	//}

	//�_�ł�����B
	Alpha();

	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_A) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_B) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_X) || INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_Y) || KeyBoardInput->isPush(DIK_RETURN);

	//�v���X�G�j�C�{�^���̏���
	if (!_AnyButton && flag)
	{
		_Start->SetActive(false);
		_NewGame->SetActive(true);
		_Continue->SetActive(true);
		_StartBar->transform->SetPosition(455, 405, 0);
		_AnyButton = true;
		_StartSE->Play(false);
		flag = false;
	}

	//�R���e�B�j���[���j���[�Q�[���̏���
	if (_AnyButton)
	{
		
		if (INSTANCE(InputManager)->GetXInput(0)->IsPressAnalog(AnalogE::L_STICKU) || KeyBoardInput->isPush(DIK_UP))
		{
			_Select = Select::NewGame;
			_StartBar->transform->SetPosition(455, 405, 0);
		}
		if (INSTANCE(InputManager)->GetXInput(0)->IsPressAnalog(AnalogE::L_STICKD) || KeyBoardInput->isPush(DIK_DOWN))
		{
			_Select = Select::Continue;
			_StartBar->transform->SetPosition(455, 475, 0);
		}

		//�X�^�[�g
		if (flag)
		{
			if (_Select == Select::Continue)
			{
				JsonData PlayerData;
				if (!PlayerData.Load("Player"))
				{
					return;
				}
				IS_CONTINUE = true;
			}
			else
			{
				IS_CONTINUE = false;
			}
			//�Q�[���V�[���ֈڍs
			_StartSE->Play(false);
			INSTANCE(SceneManager)->ChangeScene("GameScene",true);
			_AnyButton = false;
		}
	}
}

void TitleScene::Alpha()
{
	_StartColor.a -= _StartAlpha;
	_StartBar->SetBlendColor(_StartColor);
	if (_StartColor.a <= 0.2f || _StartColor.a >= 0.5f)
	{
		_StartAlpha *= -1;
	}
}
