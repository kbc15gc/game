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
	//テキストボックスを出す。
	_TextBox = INSTANCE(GameObjectManager)->AddNew<TextBox>("TextBox",StatusWindow::WindowBackPriorty - 1);
	_TextBox->SetTextSpeed(12.0f);
	_IsSpeak = false;
	_State = State::Idol;

	_Anim->PlayAnimation(static_cast<int>(State::Idol), 0.2f);
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
	_TextBox->id = info->ShopID;
	transform->SetLocalPosition(info->pos);
	transform->SetRotation(info->ang);
	transform->SetLocalScale(info->sca);

	LoadModel(info->filename, false);
}

void NPC::SetMesseage(const int & id, const bool show)
{
	_TextBox->SetMessageID(id);
	_ShowTitle = show;
}

bool NPC::GetIsSpeakEnd()const {
	return _TextBox->IsLastMessage();
}

void NPC::_Speak()
{
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
			if (_State != State::Speak && _IsAnimation)
			{
				_State = State::Speak;
				_Anim->PlayAnimation(static_cast<int>(State::Speak), 0.2f);

			}
		}
	}
	else
	{
		//離れたなら閉じる
		_TextBox->CloseMessage();
		if (_State != State::Idol && _IsAnimation)
		{
			_State = State::Idol;
			_Anim->PlayAnimation(static_cast<int>(State::Idol), 0.2f);
		}
	}
}