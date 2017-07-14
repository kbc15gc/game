#pragma once

#include "EnemyState.h"

// �G�l�~�[�U���X�e�[�g�B
class EnemyAttackState : public EnemyState {
public:
	EnemyAttackState(EnemyCharacter* Object);
	~EnemyAttackState();
	void Exit(EnemyCharacter::State next)override;

	// �U�������N���X��ݒ�B
	// ���n���ꂽ�U�������̃X�e�[�g�Ŏ��s�����B
	inline void SetAttack(EnemyAttack* attack) {
		_attack = attack;
	}

	inline bool IsPossibleChangeState(EnemyCharacter::State next)override {
		return _attack->IsPossibleChangeState(next);
	}
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);
private:
	EnemyAttack* _attack = nullptr;	// ���s�������U�������B
};