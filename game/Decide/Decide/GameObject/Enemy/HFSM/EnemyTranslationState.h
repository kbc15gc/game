#pragma once

#include "EnemyState.h"

class EnemyCharacter;

// エネミー直進ステート。
class EnemyTranslationState : public EnemyState {
public:
	EnemyTranslationState(EnemyCharacter* Object);
	~EnemyTranslationState();


	void Exit(EnemyCharacter::State next)override;


	// 移動する方向を指定。
	inline void SetDir(const Vector3& dir) {
		_Direction = dir;
		// 正規化。
		_Direction.Normalize();
	}

	// 移動量設定。
	inline void SetLength(const float length) {
		_MoveLength = length;
	}

	// 移動速度設定(毎秒)。
	inline void SetMoveSpeed(const float speed) {
		_MoveSpeed = speed;
	}

	// 移動時間設定(秒)。
	inline void SetMoveTime(const float time) {
		_MoveTime = time;
	}
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State now);

private:
	Vector3 _Direction;	// 進む方向。
	float _MoveLength;	// 移動量。
	float _MoveSpeed;	// 移動するスピード(毎秒)。
	float _MoveTime;	// 移動時間(秒)。
};