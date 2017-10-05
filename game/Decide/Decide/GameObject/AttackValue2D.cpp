#include"stdafx.h"
#include "GameObject\AttackValue2D.h"

void AttackValue2D::Awake()
{
	_Time = 0;
	_Up = 0.0f;
	_CreatAttackValue = false;
}

void AttackValue2D::Update()
{
	_Time += Time::DeltaTime();
	if (_CreatAttackValue == true)
	{
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
			if (_CriticalText)
			{
				INSTANCE(GameObjectManager)->AddRemoveList(_CriticalText);
			}
			if (_AttackText)
			{
				INSTANCE(GameObjectManager)->AddRemoveList(_AttackText);
			}
			INSTANCE(GameObjectManager)->AddRemoveList(this);
		}
	}
}

void AttackValue2D::LateUpdate()
{
}

void AttackValue2D::Init(int damagevalue, bool critical, float lifetime, Vector3 offset, Color color)
{
	//�U�����̒l��\������e�L�X�g�B
	_AttackText = INSTANCE(GameObjectManager)->AddNew<TextObject>("AttackValue", _Priority);
	//�N���e�B�J���B
	if (critical)
	{
		//�N���e�B�J���̏ꍇ
		//�F��ς���
		_AttackText->Initialize(L"", 70.0f, Color::yellow);
		//�N���e�B�J���e�L�X�g�\��
		_CriticalText = INSTANCE(GameObjectManager)->AddNew<TextObject>("CriticalText", _Priority);
		_CriticalText->Initialize(L"Critical", 70.0f, Color::yellow);
		_CriticalText->SetAnchor(fbText::TextAnchorE::UpperCenter);
	}
	else
	{
		_AttackText->Initialize(L"", 40.0f, color);
	}
	_AttackText->SetAnchor(fbText::TextAnchorE::MiddleCenter);
	//�I�t�Z�b�g�ݒ�
	_Offset = offset;
	_Pos = transform->GetPosition() + _Offset;
	//�X�N���[�����W�ɕϊ�
	Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(_Pos);
	_Pos = Vector3(_ScreenPos.x, _ScreenPos.y, 0.0f);
	//�U���e�L�X�g���W�ݒ�
	_AttackText->transform->SetLocalPosition(_Pos);
	if (_CriticalText)
	{
		//�N���e�B�J���e�L�X�g���W�ݒ�
		_CriticalText->transform->SetParent(_AttackText->transform);
		_CriticalText->transform->SetLocalPosition(0.0f, -30.0f, 0.0f);
	}
	//�_���[�W��
	char attack[4];
	sprintf(attack, "%d", damagevalue);
	_AttackText->SetText(attack);
	_AttackText->SetEffectFlg(fbSprite::SpriteEffectE::OUTLINE);
	//�������Ԑݒ�
	_LifeTime = lifetime;
	//�A�b�v�f�[�g�J�n
	_CreatAttackValue = true;
}