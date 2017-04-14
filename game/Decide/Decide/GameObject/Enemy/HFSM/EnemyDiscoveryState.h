#pragma once

#include "EnemyState.h"

class EnemyCharacter;
class Player;

// エネミー発見ステート。
class EnemyDiscoveryState : public EnemyState {
public:
	EnemyDiscoveryState(EnemyCharacter* Object);
	~EnemyDiscoveryState();
	void Exit(EnemyCharacter::State next)override;
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State now);
private:
	GameObject* _Player = nullptr;
	float _Speed = 0.0f;	// 移動速度。
};