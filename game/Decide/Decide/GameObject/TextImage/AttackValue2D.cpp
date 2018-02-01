#include"stdafx.h"
#include "AttackValue2D.h"
#include "GameObject\Village\EventManager.h"

void AttackValue2D::Awake()
{
	_Time = 0;
	_Up = 0.0f;
}

void AttackValue2D::Update()
{
	if (this->GetActive() == true && INSTANCE(EventManager)->IsEvent())
	{
		_AttackText->SetActive(false);
	}
	else
	{
		_AttackText->SetActive(true);
	}
	if (!_AttackText->GetActive())
	{
		return;
	}
	_Time += Time::DeltaTime();
	//�������Ԉȓ��Ȃ�
	if (_Time < _LifeTime)
	{
		_Up -= 1.0f;
		//�X�N���[�����W�ɕϊ�
		Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(trans->GetPosition() + _Offset);
		//�U���e�L�X�g���W�ݒ�
		_AttackText->transform->SetPosition(_ScreenPos.x, _ScreenPos.y + _Up, 0.0f);

		//if (_sonotaText) {
		//	_sonotaText->transform->SetPosition(_ScreenPos.x, _ScreenPos.y + _Up, 0.0f);
		//}
	}
	//�������Ԃ��߂����B
	else
	{
		// �폜�B
		if (_AttackText)
		{
			INSTANCE(GameObjectManager)->AddRemoveList(_AttackText);
		}
		if (_sonotaText) {
			INSTANCE(GameObjectManager)->AddRemoveList(_sonotaText);
		}
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
}

void AttackValue2D::LateUpdate()
{
}

void AttackValue2D::Init(Transform* t, int damagevalue, bool critical, float lifetime, const Vector3& offset, const Color& color, const wchar_t* text, const Vector2& pos, float size, const Color& textColor)
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
	//�e�q�֌W
	trans = t;
	transform->SetParent(trans);
	//�X�N���[�����W�ɕϊ�
	Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(trans->GetPosition() + _Offset);
	//�U���e�L�X�g���W�ݒ�
	_AttackText->transform->SetPosition(_ScreenPos.x, _ScreenPos.y, 0.0f);
	//�_���[�W��
	char attack[6];
	//���E�_���[�W��9999

	if (damagevalue >= 0) {
		// �_���[�W�ʂ�\������B

		if (damagevalue > 9999)
		{
			sprintf(attack, "%d", 9999);

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
	}

	_AttackText->SetEffectFlg(fbSprite::SpriteEffectE::OUTLINE);
	//�������Ԑݒ�
	_LifeTime = lifetime;

	if (wcscmp(text, L"")) {
		// �����������ݒ肳��Ă���΍쐬�B
		_sonotaText = INSTANCE(GameObjectManager)->AddNew<TextObject>("AttackValueSonota", _Priority);
		_sonotaText->Initialize(text, size, textColor);
		_sonotaText->SetAnchor(fbText::TextAnchorE::MiddleCenter);
		_sonotaText->transform->SetParent(_AttackText->transform);
		_sonotaOffset = pos;
		_sonotaText->transform->SetLocalPosition(pos);
		_sonotaText->SetEffectFlg(fbSprite::SpriteEffectE::OUTLINE);
	}
}