#pragma once
#include "EnemyState.h"

// ラスボスダウン(魔王行動不能、舞台装置解除、魔王のバフ全解除、魔王へのダメージ全クリティカル)ステート。
class LastBossDownState : public EnemyState {
public:
	LastBossDownState(EnemyCharacter* Object);
	~LastBossDownState();
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	bool _isOutsideRange = false;	// 範囲外に出たか。
};