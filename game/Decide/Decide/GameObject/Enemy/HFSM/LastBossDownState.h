#pragma once
#include "EnemyState.h"

// ���X�{�X�_�E��(�����s���s�\�A���䑕�u�����A�����̃o�t�S�����A�����ւ̃_���[�W�S�N���e�B�J��)�X�e�[�g�B
class LastBossDownState : public EnemyState {
public:
	LastBossDownState(EnemyCharacter* Object);
	~LastBossDownState();
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	bool _isOutsideRange = false;	// �͈͊O�ɏo�����B
};