#pragma once

#include "EnemyState.h"

class Player;

// エネミー追跡ステート。
class EnemyChaceState : public EnemyState {
public:
	EnemyChaceState(EnemyCharacter* Object);
	~EnemyChaceState();

	inline void SetEndRange(float range) {
		_endRange = range;
	}


private:
	void _EntrySubClass()override;

	void _StartSubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	bool IsPossibleChangeState(EnemyCharacter::State next)override;


private:
	Player* _Player = nullptr;
	float _Speed = 0.0f;	// 移動速度。
	bool _isOutside = false;
	float _endRange = 0.0f;	// 追跡終了距離。
	bool _isRotationinterpolate = false;// 回転を補間するか。
};