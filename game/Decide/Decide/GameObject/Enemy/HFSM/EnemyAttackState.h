#pragma once

#include "EnemyState.h"

class EnemyCharacter;

// �G�l�~�[�U���X�e�[�g�B
class EnemyAttackState : public EnemyState {
public:
	EnemyAttackState(EnemyCharacter* Object);
	~EnemyAttackState();
	void Exit(EnemyCharacter::State next)override;
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);
};