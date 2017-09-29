#include "stdafx.h"
#include "NPC.h"

#include "GameObject\Player\Player.h"
#include "GameObject\Village\TextBox.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "GameObject\StatusWindow\StatusWindow.h"


NPC::NPC(const char * name):
	ContinentObject(name),
	_Height(1.5f),
	_Radius(4.0f)
{
}

NPC::~NPC()
{
	INSTANCE(GameObjectManager)->AddRemoveList(_TextBox);
}

void NPC::Awake()
{
	ContinentObject::Awake();
	//テキストボックスを出す。
	_TextBox = INSTANCE(GameObjectManager)->AddNew<TextBox>("TextBox",StatusWindow::WindowBackPriorty - 1);
	_TextBox->SetTextSpeed(12.0f);
}

void NPC::Update()
{
	//話す。
	_Speak();
}

void NPC::LateUpdate()
{
	//頭の場所計算
	Vector3 headPos = transform->GetPosition();
	headPos.y += _Height;
	Vector2 screemPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(headPos);
	//テキストの場所設定。
	_TextBox->transform->SetPosition(Vector3(screemPos, 0));
}

void NPC::SetMesseage(const int & id, const bool show)
{
	_TextBox->SetMessageID(id);
	_ShowTitle = show;
}

void NPC::_Speak()
{
	if (_Player == nullptr)
	{
		_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	}
	else
	{
		//プレイヤーの向き
		Vector3 pDir = _Player->transform->Direction(Vector3::front);
		pDir.Normalize();
		//プレイヤーから自分への向き。
		Vector3 toMeDir = transform->GetPosition() - _Player->transform->GetPosition();
		float len = toMeDir.Length();
		toMeDir.Normalize();

		//会話可能な距離か？
		if (len <= _Radius)
		{
			if (_Player->GetState() != Player::State::Speak && _Player->GetState() != Player::State::Stop)
			{
				_Player->ChangeState(Player::State::Speak);
			}
			//タイトル表示
			if (_ShowTitle)
			{
				_TextBox->Title(true);
			}
			//本来は外部から呼び出す。
			if (KeyBoardInput->isPush(DIK_SPACE) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
			{
				//会話する。
				_TextBox->Speak();
			}
		}
		else
		{
			if (_Player->GetState() == Player::State::Speak)
			{
				_Player->ChangeState(Player::State::Run);
			}
			
			//離れたなら閉じる
			_TextBox->CloseMessage();
		}
	}
}
