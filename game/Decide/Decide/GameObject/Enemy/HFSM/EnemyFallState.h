#pragma once
#include "EnemyState.h"


// エネミー落下ステート。
class EnemyFallState :public EnemyState{
public:
	EnemyFallState(EnemyCharacter* Object);
	~EnemyFallState();
private:
	void _EntrySubClass()override;

	void _StartSubClass()override;

	void _UpdateSubClass()override;
	
	void _ExitSubClass(EnemyCharacter::State next)override {};

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	// このステートから移行できるステートを選別する関数。
	inline bool IsPossibleChangeState(EnemyCharacter::State next)override {
		if (next == EnemyCharacter::State::Damage) {
			return false;
		}
		return true;
	}

	// このステート中にダメージを与えられるか。
	inline virtual bool IsPossibleDamage() {
		return false;
	}

};