#pragma once
#include "EnemyState.h"


// エネミー落下ステート。
class EnemyFallState :public EnemyState{
public:
	EnemyFallState(EnemyCharacter* Object);
	~EnemyFallState();
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;
	
	void _ExitSubClass(EnemyCharacter::State next)override {};

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);
};