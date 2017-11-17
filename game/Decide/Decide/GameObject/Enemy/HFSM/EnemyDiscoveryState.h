#pragma once

#include "EnemyState.h"

class Player;

// エネミー発見ステート。
class EnemyDiscoveryState : public EnemyState {
public:
	EnemyDiscoveryState(EnemyCharacter* Object);
	~EnemyDiscoveryState();
private:
	void _EntrySubClass()override;

	void _StartSubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	bool IsPossibleChangeState(EnemyCharacter::State next)override;

private:
};