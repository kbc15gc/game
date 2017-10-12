#pragma once

#include "EnemyState.h"

class Player;

// エネミー死亡ステート。
class EnemyDeathState : public EnemyState {
public:
	EnemyDeathState(EnemyCharacter* Object);
	~EnemyDeathState();
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	inline bool IsPossibleChangeState(EnemyCharacter::State next) {
		return false;
	}
public:
	inline void SetWaitTime(float time) {
		_waitTime = time;
	}
private:
	float _waitTime = 0.0f;	// 死亡モーション再生終了から削除までの待ち時間。
	float _timeCounter = 0.0f;
	bool _isEndAnim = false;	// 再生終了フラグ。
};