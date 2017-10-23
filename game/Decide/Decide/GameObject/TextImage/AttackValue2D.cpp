#include"stdafx.h"
#include "AttackValue2D.h"

void AttackValue2D::Awake()
{
	_Time = 0;
	_Up = 0.0f;
}

void AttackValue2D::Update()
{
	_Time += Time::DeltaTime();
	//�������Ԉȓ��Ȃ�
	if (_Time < _LifeTime)
	{
		_Up += 0.01f;
		_Pos = transform->GetPosition() + _Offset;
		_Pos.y += _Up;
		Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(_Pos);
		_Pos = Vector3(_ScreenPos.x, _ScreenPos.y, 0.0f);
		_AttackText->transform->SetLocalPosition(_Pos);
	}
	//�������Ԃ��߂����B
	else
	{
		// �폜�B
		if (_AttackText)
		{
			INSTANCE(GameObjectManager)->AddRemoveList(_AttackText);
		}
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
}

void AttackValue2D::LateUpdate()
{
}

void AttackValue2D::Init(int damagevalue, bool critical, float lifetime, Vector3 offset, Color color)
{
	//�U�����̒l��\������e�L�X�g�B
	if (critical)
	{
		_AttackText = INSTANCE(GameObjectManager)->AddNew<TextObject>("AttackValue", _Priority);
		_AttackText->Initialize(L"", 70.0f, Color::yellow);
		_AttackText->SetAnchor(fbText::TextAnchorE::MiddleCenter);
	}
	else
	{
		_AttackText = INSTANCE(GameObjectManager)->AddNew<TextObject>("AttackValue", _Priority);
		_AttackText->Initialize(L"", 40.0f, color);
		_AttackText->SetAnchor(fbText::TextAnchorE::MiddleCenter);
	}
	
	//�I�t�Z�b�g�ݒ�
	_Offset = offset;
	_Pos = transform->GetPosition() + _Offset;
	//�X�N���[�����W�ɕϊ�
	Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(_Pos);
	_Pos = Vector3(_ScreenPos.x, _ScreenPos.y, 0.0f);
	//�U���e�L�X�g���W�ݒ�
	_AttackText->transform->SetLocalPosition(_Pos);
	//�_���[�W��
	char attack[6];
	//���E�_���[�W��999
	if (damagevalue > 999)
	{
		sprintf(attack, "%d", 999);

	}
	else
	{
		sprintf(attack, "%d", damagevalue);

	}

	//�N���e�B�J���B
	if (critical)
	{
		//�N���e�B�J���̏ꍇ
		char* crt = "!";
		strcat(attack, crt);
	}

	_AttackText->SetText(attack);
	_AttackText->SetEffectFlg(fbSprite::SpriteEffectE::OUTLINE);
	//�������Ԑݒ�
	_LifeTime = lifetime;
}