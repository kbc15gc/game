#pragma once

#include "EnemyState.h"


// エネミー攻撃開始ステート。
class EnemyStartAttackState : public EnemyState {
public:
	EnemyStartAttackState(EnemyCharacter* Object);
	~EnemyStartAttackState();
private:
	void _EntrySubClass()override;

	void _StartSubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	inline bool IsPossibleChangeState(EnemyCharacter::State next)override {
		if (_NowLocalState) {
			return _NowLocalState->IsPossibleChangeState(next);
		}
		return true;
	}

private:
	GameObject* _Player = nullptr;
};