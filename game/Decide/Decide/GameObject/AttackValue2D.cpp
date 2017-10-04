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
		

		//生存時間以内なら
		if (_Time < _LifeTime)
		{	
			_Up += 0.01f;
			_Pos = transform->GetPosition() + _Offset;
			_Pos.y += _Up;
			Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(_Pos);
			_Pos = Vector3(_ScreenPos.x, _ScreenPos.y, 0.0f);
			_AttackText->transform->SetLocalPosition(_Pos);
		}
		//生存時間を過ぎた。
		else
		{
			// 削除。
			INSTANCE(GameObjectManager)->AddRemoveList(_AttackText);
			INSTANCE(GameObjectManager)->AddRemoveList(this);
		}
	}
}

void AttackValue2D::LateUpdate()
{
}

void AttackValue2D::Init(int damagevalue, float lifetime, Vector3 offset, Color color)
{
	//攻撃時の値を表示するテキスト。
	_AttackText = INSTANCE(GameObjectManager)->AddNew<TextObject>("AttackValue", _Priority);
	_AttackText->Initialize(L"", 40.0f,color);
	_AttackText->SetAnchor(fbText::TextAnchorE::Middle);
	_Offset = offset;
	_Pos = transform->GetPosition() + _Offset;

	Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(_Pos);
	_Pos = Vector3(_ScreenPos.x, _ScreenPos.y, 0.0f);
	_AttackText->transform->SetLocalPosition(_Pos);
	char attack[4];
	sprintf(attack, "%d", damagevalue);
	_AttackText->SetText(attack);

	_LifeTime = lifetime;
	_CreatAttackValue = true;
}