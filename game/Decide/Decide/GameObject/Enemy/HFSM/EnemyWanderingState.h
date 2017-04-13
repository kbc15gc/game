#pragma once
#include "EnemyState.h"

class EnemyCharacter;

// エネミー徘徊ステート。
class EnemyWanderingState : public EnemyState {
public:
	EnemyWanderingState(EnemyCharacter* Object);
	~EnemyWanderingState();
	void Entry()override;
	void Exit(EnemyCharacter::State next)override;
private:
	void _UpdateSubClass()override;
	void _EndNowLocalState_CallBack(EnemyCharacter::State now);
};