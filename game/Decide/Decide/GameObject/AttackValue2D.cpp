#include"stdafx.h"
#include "GameObject\AttackValue2D.h"

void AttackValue2D::Awake()
{
	_Time = 0;
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
			_AttackText->transform->SetPosition(_Pos.x, _Pos.y -= 1.0f, 0.0f);

		}
		//�������Ԃ��߂����B
		else
		{
			// �폜�B
			INSTANCE(GameObjectManager)->AddRemoveList(_AttackText);
			INSTANCE(GameObjectManager)->AddRemoveList(this);
		}
	}
}

void AttackValue2D::LateUpdate()
{

}

void AttackValue2D::Init(Vector3 pos, int damagevalue, float lifetime, Vector3 offset)
{
	//�U�����̒l��\������e�L�X�g�B
	_AttackText = INSTANCE(GameObjectManager)->AddNew<TextObject>("AttackValue", _Priority);
	_AttackText->Initialize(L"", 70.0f);
	_AttackText->SetFormat((int)fbText::TextFormatE::CENTER | (int)fbText::TextFormatE::UP);
	_Pos = pos + offset;

	Camera* camera = INSTANCE(GameObjectManager)->mainCamera;

	Vector2 _ScreenPos = camera->WorldToScreen(_Pos);
	_Pos = Vector3(_ScreenPos.x, _ScreenPos.y, 0.0f);

	_AttackText->transform->SetPosition(_Pos);
	string attackvalue = to_string(damagevalue);
	_AttackText->SetString(attackvalue.data());

	_LifeTime = lifetime;
	_CreatAttackValue = true;
}