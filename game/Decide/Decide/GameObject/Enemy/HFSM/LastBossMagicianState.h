#pragma once
#include "EnemyState.h"

// ���X�{�X���p�t(�������ڐ퓬�A�����Ƀo�t�A�_���[�W�v�Z�ʏ�)�X�e�[�g�B
class LastBossMagicianState : public EnemyState {
public:
	LastBossMagicianState(EnemyCharacter* Object);
	~LastBossMagicianState();
	void Exit(EnemyCharacter::State next)override;
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	bool _isOutsideRange = false;	// �͈͊O�ɏo�����B
};