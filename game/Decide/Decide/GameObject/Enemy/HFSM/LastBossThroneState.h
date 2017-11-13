#pragma once
#include "EnemyState.h"

class BossGhost;

// ラスボス玉座(側近召喚、プレイヤーにデバフ、魔王無敵状態)ステート。
class LastBossThroneState : public EnemyState {
public:
	LastBossThroneState(EnemyCharacter* Object);
	~LastBossThroneState();
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	bool _isOutsideRange = false;	// 範囲外に出たか。
	float _timeCounter;
	float _interval = 20.0f;
	bool _isCommand = false;

	static const int _entourageNum;	// 側近の数。
	vector<BossGhost*> _entourageEnemys;	// 側近。
};