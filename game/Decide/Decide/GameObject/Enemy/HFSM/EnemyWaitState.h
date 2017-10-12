#pragma once

#include "EnemyState.h"

// エネミー待機ステート。
class EnemyWaitState : public EnemyState {
public:
	EnemyWaitState(EnemyCharacter* Object);
	~EnemyWaitState();

	// 待機時間設定。
	inline void SetInterval(const float time) {
		_Interval = time;
	}
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override {};

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);
private:
	float _TimeCounter;	// 作業用。
	float _Interval;	// 待機時間。
};