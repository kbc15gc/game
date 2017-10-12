#pragma once
#include "EnemyState.h"

// ラスボス魔術師(魔王直接戦闘、魔王にバフ、ダメージ計算通常)ステート。
class LastBossMagicianState : public EnemyState {
public:
	LastBossMagicianState(EnemyCharacter* Object);
	~LastBossMagicianState();
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	bool _isOutsideRange = false;	// 範囲外に出たか。
	float _timeCounter;
	float _interval = 9999.0f;	// 大魔法に移行するまでの時間。
};