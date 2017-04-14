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
		Death = 0,
		Damage,
		Attack,
		Walk,
		Stand,
		Max
	};
public:
	Enemy(const char* name);
	~Enemy();
protected:
	void _EndNowStateCallback(State EndStateType)override;

private:
	void _AwakeSubClass()override;
	void _StartSubClass()override;
	void _UpdateSubClass()override;

	// コリジョン定義関数。
	// コリジョンの形状やパラメータを設定する関数。
	void _ConfigCollision()override;

	// 継承先でアニメーション番号のテーブルを作成。
	// ※添え字にはこのクラス定義したAnimationType列挙体を使用。
	void _BuildAnimation()override;
};

