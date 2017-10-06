#include "stdafx.h"
#include "NPC.h"

#include "GameObject\Player\Player.h"
#include "GameObject\Village\TextBox.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "GameObject\StatusWindow\StatusWindow.h"


NPC::NPC(const char * name):
	ContinentObject(name),
	_Height(1.5f),
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
	//�e�L�X�g�{�b�N�X���o���B
	_TextBox = INSTANCE(GameObjectManager)->AddNew<TextBox>("TextBox",StatusWindow::WindowBackPriorty - 1);
	_TextBox->SetTextSpeed(12.0f);
	_IsSpeak = false;
}

void NPC::Update()
{
	//�b���B
	_Speak();
}

void NPC::LateUpdate()
{
	//���̏ꏊ�v�Z
	Vector3 headPos = transform->GetPosition();
	headPos.y += _Height;
	Vector2 screemPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(headPos);
	//�e�L�X�g�̏ꏊ�ݒ�B
	_TextBox->transform->SetPosition(Vector3(screemPos, 0));
}

void NPC::SetMesseage(const int & id, const bool show)
{
	_TextBox->SetMessageID(id);
	_ShowTitle = show;
}

void NPC::_Speak()
{
	//��b�\�ȋ������H
	if (_IsSpeak)
	{
		//�^�C�g���\��
		if (_ShowTitle)
		{
			_TextBox->Title(true);
		}
		//�{���͊O������Ăяo���B
		if (VPadInput->IsPush(fbEngine::VPad::ButtonA))
		{
			//��b����B
			_TextBox->Speak();
		}
	}
	else
	{
		//���ꂽ�Ȃ����
		_TextBox->CloseMessage();
	}
}