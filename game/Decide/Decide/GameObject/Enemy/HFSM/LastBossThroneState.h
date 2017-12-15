#pragma once
#include "EnemyState.h"

class BossGhost;
class Player;

// ラスボス玉座(側近召喚、プレイヤーにデバフ、魔王無敵状態)ステート。
class LastBossThroneState : public EnemyState {
public:
	LastBossThroneState(EnemyCharacter* Object);
	~LastBossThroneState();

	// 側近に共同攻撃命令。
	bool EntourageCommand();
	// 側近にバフ。
	void EncourageBuff();
private:
	void _EntrySubClass()override;

	void _StartSubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	// このステートから移行できるステートを選別する関数。
	inline virtual bool IsPossibleChangeState(EnemyCharacter::State next) {
		if (next == EnemyCharacter::State::Damage || next == EnemyCharacter::State::Death || next == EnemyCharacter::State::Fall) {
			return false;
		}
		return true;
	}

	// このステート中にダメージを与えられるか。
	inline virtual bool IsPossibleDamage() {
		return false;
	}


private:
	bool _isOutsideRange = false;	// 範囲外に出たか。
	float _timeCounter;
	float _interval = 10.0f;

	bool _isDeathEntourage = false;
	static const int _entourageNum;	// 側近の数。
	vector<BossGhost*> _entourageEnemys;	// 側近。
	Player* _player = nullptr;
};