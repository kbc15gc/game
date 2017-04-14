#pragma once

#include "EnemyState.h"

class EnemyCharacter;

// �G�l�~�[�U���J�n�X�e�[�g�B
class EnemyStartAttackState : public EnemyState {
public:
	EnemyStartAttackState(EnemyCharacter* Object);
	~EnemyStartAttackState();
	void Exit(EnemyCharacter::State next)override;
private:
	void _EntrySubClass()override;

	void _Start()override;

	// �x�̍U�������邩���肷��֐��B
	// �U���p�^�[���𑝂₵���ۂɂ͂����ɕ�����L�ځB
	// ����p��AI�����������ŌĂяo�����L�ڂ���B
	void _AttackSelect();

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);
};