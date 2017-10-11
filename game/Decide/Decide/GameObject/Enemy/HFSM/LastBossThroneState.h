#pragma once
#include "EnemyState.h"

// ラスボス玉座(側近召喚、プレイヤーにデバフ、魔王無敵状態)ステート。
class LastBossThroneState : public EnemyState {
public:
	LastBossThroneState(EnemyCharacter* Object);
	~LastBossThroneState();
	void Exit(EnemyCharacter::State next)override;
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	bool _isOutsideRange = false;	// 範囲外に出たか。
};