#pragma once

#include "EnemyState.h"


// エネミーダメージ反応ステート。
class EnemyDamageReactionState : public EnemyState {
public:
	EnemyDamageReactionState(EnemyCharacter* Object) : EnemyState(Object) {};
	~EnemyDamageReactionState() {};
private:
	void _EntrySubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override {};

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	inline bool IsPossibleChangeState(EnemyCharacter::State next) {
		if (next == EnemyCharacter::State::Fall || next == EnemyCharacter::State::Death) {
			return true;	// 落下と死亡のみ許可。
		}
		else {
			return false;
		}
	}

};