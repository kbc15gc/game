#pragma once

#include "EnemyState.h"
#include "GameObject\Enemy\EnemyAttack.h"

// エネミー攻撃ステート。
class EnemyAttackState : public EnemyState {
public:
	EnemyAttackState(EnemyCharacter* Object);
	~EnemyAttackState();

	inline bool IsPossibleChangeState(EnemyCharacter::State next)override {
		return _attack->IsPossibleChangeState(next);
	}
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);
private:
	EnemyAttack* _attack = nullptr;	// 実行したい攻撃処理。
};