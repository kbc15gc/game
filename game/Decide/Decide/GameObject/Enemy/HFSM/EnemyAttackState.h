#pragma once

#include "EnemyState.h"

class EnemyCharacter;

// エネミー攻撃ステート。
class EnemyAttackState : public EnemyState {
public:
	EnemyAttackState(EnemyCharacter* Object);
	~EnemyAttackState();
	void Exit(EnemyCharacter::State next)override;
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);
};