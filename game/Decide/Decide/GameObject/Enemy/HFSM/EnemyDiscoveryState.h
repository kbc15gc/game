#pragma once

#include "EnemyState.h"

class Player;

// �G�l�~�[�����X�e�[�g�B
class EnemyDiscoveryState : public EnemyState {
public:
	EnemyDiscoveryState(EnemyCharacter* Object);
	~EnemyDiscoveryState();
	void Exit(EnemyCharacter::State next)override;
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	bool IsPossibleChangeState(EnemyCharacter::State next)override;

private:
	Player* _Player = nullptr;
	float _Speed = 0.0f;	// �ړ����x�B
	bool _isOutside = false;
};