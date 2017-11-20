#pragma once
#include "EnemyState.h"

// ラスボス歴史改竄(歴史書召喚、舞台装置稼働、大魔法発動、魔王無敵状態)ステート。
class LastBossHistoryTamperingState : public EnemyState {
public:
	LastBossHistoryTamperingState(EnemyCharacter* Object);
	~LastBossHistoryTamperingState();
private:
	void _EntrySubClass()override;

	void _StartSubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	bool _isOutsideRange = false;	// 範囲外に出たか。

	vector<GameObject*> _historyBuildings;	// 舞台装置。
};