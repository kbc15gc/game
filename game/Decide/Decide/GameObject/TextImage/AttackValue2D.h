#pragma once
#include "fbEngine\_Object\_GameObject\TextObject.h"

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
	// 座標。
	// ダメージ量。
	// 表示する時間。
	// ずらす量。
	//カラー。
	void Init(int damagevalue, bool critical, float lifetime = 1.0f, Vector3 offset = Vector3::zero, Color color = Color::red);
private:
	//攻撃量のテキスト
	TextObject* _AttackText;
	//攻撃量の最大生存時間。
	float _LifeTime;
	//攻撃量の現在の時間。
	float _Time;
	//ポジション。
	Vector3 _Pos;
	//位置を基準点からの距離で表した値。
	Vector3 _Offset;
	//上昇値。
	float _Up;
};