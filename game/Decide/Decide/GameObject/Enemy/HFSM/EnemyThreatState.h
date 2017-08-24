#pragma once

#include "EnemyState.h"

// �G�l�~�[�Њd�X�e�[�g�B
class EnemyThreatState : public EnemyState {
public:
	EnemyThreatState(EnemyCharacter* Object) :EnemyState(Object) {};
	~EnemyThreatState() {};

	void Start();
	void Exit(EnemyCharacter::State next)override {};

private:
	void _EntrySubClass()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);
};