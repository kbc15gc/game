#pragma once

#include "EnemyState.h"

class Player;

// エネミー追跡ステート。
class EnemyChaceState : public EnemyState {
public:
	EnemyChaceState(EnemyCharacter* Object);
	~EnemyChaceState();
	void Exit(EnemyCharacter::State next)override;
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	bool IsPossibleChangeState(EnemyCharacter::State next)override;

private:
	Player* _Player = nullptr;
	float _Speed = 0.0f;	// 移動速度。
	bool _isOutside = false;
};