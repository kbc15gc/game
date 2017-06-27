#pragma once
#include "EnemyState.h"

// エネミー徘徊ステート。
class EnemyWanderingState : public EnemyState {
public:
	EnemyWanderingState(EnemyCharacter* Object);
	~EnemyWanderingState();
	void Exit(EnemyCharacter::State next)override;
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	// 移動ステートパラメータの保持。
	// 引数：	向き。
	//			移動量。
	//			移動速度。
	void _TranslationPalam(const Vector3& dir,float length,float speed);

private:
	bool _isOutsideRange = false;	// 範囲外に出たか。
};