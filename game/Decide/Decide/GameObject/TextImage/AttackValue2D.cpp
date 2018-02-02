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
	//生存時間以内なら
	if (_Time < _LifeTime)
	{
		_Up -= 1.0f;
		//スクリーン座標に変換
		Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(trans->GetPosition() + _Offset);
		//攻撃テキスト座標設定
		_AttackText->transform->SetPosition(_ScreenPos.x, _ScreenPos.y + _Up, 0.0f);

		//if (_sonotaText) {
		//	_sonotaText->transform->SetPosition(_ScreenPos.x, _ScreenPos.y + _Up, 0.0f);
		//}
	}
	//生存時間を過ぎた。
	else
	{
		// 削除。
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
	//親子関係
	trans = t;
	transform->SetParent(trans);
	//スクリーン座標に変換
	Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(trans->GetPosition() + _Offset);
	//攻撃テキスト座標設定
	_AttackText->transform->SetPosition(_ScreenPos.x, _ScreenPos.y, 0.0f);
	//ダメージ量
	char attack[6];
	//限界ダメージは9999

	if (damagevalue >= 0) {
		// ダメージ量を表示する。

		if (damagevalue > 9999)
		{
			sprintf(attack, "%d", 9999);

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
	}

	_AttackText->SetEffectFlg(fbSprite::SpriteEffectE::OUTLINE);
	//制限時間設定
	_LifeTime = lifetime;

	if (wcscmp(text, L"")) {
		// 文字が何か設定されていれば作成。
		_sonotaText = INSTANCE(GameObjectManager)->AddNew<TextObject>("AttackValueSonota", _Priority);
		_sonotaText->Initialize(text, size, textColor);
		_sonotaText->SetAnchor(fbText::TextAnchorE::MiddleCenter);
		_sonotaText->transform->SetParent(_AttackText->transform);
		_sonotaOffset = pos;
		_sonotaText->transform->SetLocalPosition(pos);
		_sonotaText->SetEffectFlg(fbSprite::SpriteEffectE::OUTLINE);
	}
}