#pragma once
#include "EnemyState.h"

class EnemyCharacter;

// �G�l�~�[�p�j�X�e�[�g�B
class EnemyWanderingState : public EnemyState {
public:
	EnemyWanderingState(EnemyCharacter* Object);
	~EnemyWanderingState();
	void Exit(EnemyCharacter::State next)override;
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State now);
};