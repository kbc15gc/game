#pragma once

#include "EnemyState.h"

// エネミー攻撃ステート。
class EnemyAttackState : public EnemyState {
public:
	EnemyAttackState(EnemyCharacter* Object);
	~EnemyAttackState();
	void Exit(EnemyCharacter::State next)override;

	// 攻撃処理クラスを設定。
	// ※渡された攻撃がこのステートで実行される。
	inline void SetAttack(EnemyAttack* attack) {
		_attack = attack;
	}

	inline bool IsPossibleChangeState(EnemyCharacter::State next)override {
		return _attack->IsPossibleChangeState(next);
	}
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);
private:
	EnemyAttack* _attack = nullptr;	// 実行したい攻撃処理。
};