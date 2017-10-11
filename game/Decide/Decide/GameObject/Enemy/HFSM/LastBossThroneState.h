#pragma once
#include "EnemyState.h"

// ���X�{�X�ʍ�(���ߏ����A�v���C���[�Ƀf�o�t�A�������G���)�X�e�[�g�B
class LastBossThroneState : public EnemyState {
public:
	LastBossThroneState(EnemyCharacter* Object);
	~LastBossThroneState();
	void Exit(EnemyCharacter::State next)override;
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	bool _isOutsideRange = false;	// �͈͊O�ɏo�����B
};