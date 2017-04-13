#pragma once
#include "EnemyCharacter.h"

// 継承クラス。
// 雑魚エネミー(プロト)。
class Enemy :
	public EnemyCharacter
{
private:
	// エネミー(プロト)のアニメーション番号。
	enum class AnimationProt {
		Stand = 0,
		Walk,
		Attack,
		Damage,
		Death,
		Anim
	};
public:
	Enemy(const char* name);
	~Enemy();
	void Awake()override;
	void Start()override;
	void Update()override;
private:
	// コリジョン定義関数。
	// コリジョンの形状やパラメータを設定する関数。
	void _ConfigCollision()override;

	// 継承先でアニメーション番号のテーブルを作成。
	// ※添え字にはこのクラス定義したAnimationType列挙体を使用。
	void _BuildAnimation()override;
};

