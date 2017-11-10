#pragma once

#include "EnemyState.h"

// ゴースト共同攻撃開始ステート。
class GhostPairAttackState : public EnemyState {
public:
	GhostPairAttackState(EnemyCharacter* Object);
	~GhostPairAttackState();

	inline bool GetIsEndWarp()const {
		return _isEndWarp;
	}
	inline void SetIsStartAttack(bool flg) {
		_isStartAttack = flg;
	}
	inline bool GetIsStartAttack()const {
		return _isStartAttack;
	}
	inline void SetStartAttackInterval(float time) {
		_startAttackInterval = time;
	}
	inline bool GetIsStartAttackAlready()const{
		return _isStartAttackAlready;
	}
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	inline bool IsPossibleChangeState(EnemyCharacter::State next)override {
		if (_NowLocalState) {
			return _NowLocalState->IsPossibleChangeState(next);
		}
		return true;
	}

private:
	GameObject* _Player = nullptr;
	bool _isEndWarp = false;
	bool _isStartAttack = false;
	bool _isStartAttackAlready = false;	// すでに攻撃を開始しているか。
	float _startAttackInterval = 0.0f;	// 攻撃開始命令が下されてからの待ち時間。
	float _counter = 0.0f;
};