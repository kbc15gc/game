#pragma once
#include "EnemyState.h"

// ラスボス魔術師(魔王直接戦闘、魔王にバフ、ダメージ計算通常)ステート。
class LastBossMagicianState : public EnemyState {
public:
	LastBossMagicianState(EnemyCharacter* Object);
	~LastBossMagicianState();
	void Exit(EnemyCharacter::State next)override;
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	bool _isOutsideRange = false;	// 範囲外に出たか。
};