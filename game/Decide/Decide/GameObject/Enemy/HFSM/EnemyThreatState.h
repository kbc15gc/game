#pragma once

#include "EnemyState.h"

// �G�l�~�[�Њd�X�e�[�g�B
class EnemyThreatState : public EnemyState {
public:
	EnemyThreatState(EnemyCharacter* Object) :EnemyState(Object) {
		_playAnimation = EnemyCharacter::AnimationType::Threat;
		_interpolate = 0.5f;
	};
	~EnemyThreatState() {};

	void _StartSubClass()override;

private:
	void _EntrySubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override {};

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	bool _isNotMotion = false;
};