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
		//生存時間以内なら
		if (_Time < _LifeTime)
		{	
			/*Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(_Pos);
			_Pos = Vector3(_ScreenPos.x, _ScreenPos.y, 0.0f);*/
			_Pos.y--;
			_AttackText->transform->SetPosition(_Pos.x, _Pos.y, 0.0f);

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

void AttackValue2D::Init(Vector3 pos, int damagevalue, float lifetime, Vector3 offset)
{
	//攻撃時の値を表示するテキスト。
	_AttackText = INSTANCE(GameObjectManager)->AddNew<TextObject>("AttackValue", _Priority);
	_AttackText->Initialize(L"", 40.0f,Color::red);
	_AttackText->SetFormat((int)fbText::TextFormatE::CENTER | (int)fbText::TextFormatE::UP);
	_Pos = pos + offset;

	Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(_Pos);
	_Pos = Vector3(_ScreenPos.x, _ScreenPos.y, 0.0f);
	_AttackText->transform->SetLocalPosition(_Pos);
	string attackvalue = to_string(damagevalue);
	_AttackText->SetString(attackvalue.data());

	_LifeTime = lifetime;
	_CreatAttackValue = true;
}