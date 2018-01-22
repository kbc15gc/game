#include "stdafx.h"
#include "NPC.h"

#include "GameObject\Player\Player.h"
#include "GameObject\Village\TextBox.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "GameObject\StatusWindow\StatusWindow.h"


NPC::NPC(const char * name):
	ContinentObject(name),
	_Height(1.0f),
	_Radius(3.0f)
{
}

NPC::~NPC()
{
	INSTANCE(GameObjectManager)->AddRemoveList(_TextBox);
}

void NPC::Awake()
{
	ContinentObject::Awake();

	_Rotation = AddComponent<ObjectRotation>();
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

	//テキストボックスを出す。
	_TextBox = INSTANCE(GameObjectManager)->AddNew<TextBox>("TextBox",StatusWindow::WindowBackPriorty - 1);
	_TextBox->SetTextSpeed(12.0f);
	_IsSpeak = false;
	_State = State::Idol;

	PlayAnimation(State::Idol, 0.2f);
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

void NPC::CreateNPC(const npc::NPCInfo* info)
{
	SetMesseage(info->MesseageID, info->ShowTitle);
	_TextBox->SetEventNo(info->EventNo);
	transform->SetLocalPosition(info->pos);
	transform->SetRotation(info->ang);
	transform->SetLocalScale(info->sca);

	//初期回転保存のため
	_Rot = info->ang;

	LoadModel(info->filename, false);
}

void NPC::SetMesseage(const int & id, const bool show)
{
	_TextBox->SetMessageID(id);
	_ShowTitle = show;
}

bool NPC::GetIsSpeakEnd()const {
	return _TextBox->IsMessageEnd();
}

bool NPC::GetisSpeakEndLastMessage()const {
	return _TextBox->IsLastMessageEnd();
}

void NPC::_Speak()
{
	//プレイヤーとの距離を計算。
	float len = (transform->GetPosition() - _Player->transform->GetPosition()).Length();
	if (_Player->GetNearNPCLen() > len)
	{
		_Player->SetNPC(this);
	}
	//会話可能な距離か？
	if (_IsSpeak)
	{
		//タイトル表示
		if (_ShowTitle)
		{
			_TextBox->Title(true);
		}
		//本来は外部から呼び出す。
		if (VPadInput->IsPush(fbEngine::VPad::ButtonA))
		{
			//会話する。

			_TextBox->Speak();
			_Rotation->RotationToObject_XZ(_Player);
			if (_State != State::Speak && _IsAnimation)
			{
				_Player->SetSpeakFlag(true);
				_State = State::Speak;
				PlayAnimation(State::Speak, 0.2f);
				
			}
		}
	}
	
	//ショップの場合。買い物中
	bool eventflag = INSTANCE(EventManager)->ShopEvent();

	if(!_IsSpeak || GetIsSpeakEnd())
	{
		_TextBox->CloseMessage();
		if (!eventflag)
		{
			transform->SetRotation(_Rot);
		}
		if (_State != State::Idol && _IsAnimation)
		{
			_Player->SetSpeakFlag(false);
			_State = State::Idol;
			PlayAnimation(State::Idol, 0.2f);
		}
	}
}