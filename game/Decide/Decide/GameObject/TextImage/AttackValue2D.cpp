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
	//攻撃時の値を表示するテキスト。
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
	
	//オフセット設定
	_Offset = offset;
	_Pos = transform->GetPosition() + _Offset;
	//スクリーン座標に変換
	Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(_Pos);
	_Pos = Vector3(_ScreenPos.x, _ScreenPos.y, 0.0f);
	//攻撃テキスト座標設定
	_AttackText->transform->SetLocalPosition(_Pos);
	//ダメージ量
	char attack[6];
	//限界ダメージは999
	if (damagevalue > 999)
	{
		sprintf(attack, "%d", 999);

	}
	else
	{
		sprintf(attack, "%d", damagevalue);

	}

	//クリティカル。
	if (critical)
	{
		//クリティカルの場合
		char* crt = "!";
		strcat(attack, crt);
	}

	_AttackText->SetText(attack);
	_AttackText->SetEffectFlg(fbSprite::SpriteEffectE::OUTLINE);
	//制限時間設定
	_LifeTime = lifetime;
}