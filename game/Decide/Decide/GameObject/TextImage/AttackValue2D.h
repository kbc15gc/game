#pragma once
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Village\EventManager.h"

class AttackValue2D : public GameObject
{
public:
	AttackValue2D(const char * name) :GameObject(name)
	{
	}
	~AttackValue2D() {
	}

	void Awake()override;
	void Update()override;
	void LateUpdate()override;
	//ダメージ量を出す２Dの初期化
	//ダメージを出すオブジェクトのトランスフォーム
	// ダメージ量(0より小さい値で数字非表示)。
	// クリティカルか。
	// 表示する時間。
	// ずらす量。
	//カラー。
	// その他表示したい文字。
	// その他表示したい文字の位置(数字からの差分、デフォルトは重なって表示、数字を表示しない場合も文字のみ出せる)。
	// その他表示したい文字のテキストサイズ。
	// その他表示したい文字のテキストカラー。
	void Init(Transform* t,int damagevalue, bool critical, float lifetime = 1.0f, const Vector3& offset = Vector3::zero, const Color& color = Color::red,const wchar_t* text = L"",const Vector2& pos = Vector2::zero,float size = 30.0f,const Color& textColor = Color::white);
private:
	//攻撃量のテキスト
	TextObject* _AttackText = nullptr;
	TextObject* _sonotaText = nullptr;	// その他表示したいもの。
	//攻撃量の最大生存時間。
	float _LifeTime;
	//攻撃量の現在の時間。
	float _Time;
	//位置を基準点からの距離で表した値。
	Vector3 _Offset;
	Vector2 _sonotaOffset;
	//上昇値
	float _Up;
	//オブジェクトのトランスフォーム
	Transform* trans;
};