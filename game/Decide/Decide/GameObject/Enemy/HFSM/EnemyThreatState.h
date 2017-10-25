#pragma once

#include "EnemyState.h"

// �G�l�~�[�Њd�X�e�[�g�B
class EnemyThreatState : public EnemyState {
public:
	EnemyThreatState(EnemyCharacter* Object) :EnemyState(Object) {};
	~EnemyThreatState() {};

	void Start();

private:
	void _EntrySubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override {};

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	bool _isNotMotion = false;
};