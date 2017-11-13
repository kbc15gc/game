#pragma once

#include "EnemyState.h"

// �S�[�X�g�����U���J�n�X�e�[�g�B
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
	bool _isStartAttackAlready = false;	// ���łɍU�����J�n���Ă��邩�B
	float _startAttackInterval = 0.0f;	// �U���J�n���߂�������Ă���̑҂����ԁB
	float _counter = 0.0f;
};