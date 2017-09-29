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
	//�e�L�X�g�{�b�N�X���o���B
	_TextBox = INSTANCE(GameObjectManager)->AddNew<TextBox>("TextBox",StatusWindow::WindowBackPriorty - 1);
	_TextBox->SetTextSpeed(12.0f);
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
	if (_Player == nullptr)
	{
		_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	}
	else
	{
		//�v���C���[�̌���
		Vector3 pDir = _Player->transform->Direction(Vector3::front);
		pDir.Normalize();
		//�v���C���[���玩���ւ̌����B
		Vector3 toMeDir = transform->GetPosition() - _Player->transform->GetPosition();
		float len = toMeDir.Length();
		toMeDir.Normalize();

		//��b�\�ȋ������H
		if (len <= _Radius)
		{
			if (_Player->GetState() != Player::State::Speak && _Player->GetState() != Player::State::Stop)
			{
				_Player->ChangeState(Player::State::Speak);
			}
			//�^�C�g���\��
			if (_ShowTitle)
			{
				_TextBox->Title(true);
			}
			//�{���͊O������Ăяo���B
			if (KeyBoardInput->isPush(DIK_SPACE) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
			{
				//��b����B
				_TextBox->Speak();
			}
		}
		else
		{
			if (_Player->GetState() == Player::State::Speak)
			{
				_Player->ChangeState(Player::State::Run);
			}
			
			//���ꂽ�Ȃ����
			_TextBox->CloseMessage();
		}
	}
}
