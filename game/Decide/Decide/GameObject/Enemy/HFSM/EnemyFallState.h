#pragma once
#include "EnemyState.h"

class EnemyCharacter;

// エネミー落下ステート。
class EnemyFallState :public EnemyState{
public:
	EnemyFallState(EnemyCharacter* Object);
	~EnemyFallState();
	void Exit(EnemyCharacter::State next)override;
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);
};