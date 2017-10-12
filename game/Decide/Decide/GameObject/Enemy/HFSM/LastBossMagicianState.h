#pragma once
#include "EnemyState.h"

// ���X�{�X���p�t(�������ڐ퓬�A�����Ƀo�t�A�_���[�W�v�Z�ʏ�)�X�e�[�g�B
class LastBossMagicianState : public EnemyState {
public:
	LastBossMagicianState(EnemyCharacter* Object);
	~LastBossMagicianState();
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	bool _isOutsideRange = false;	// �͈͊O�ɏo�����B
	float _timeCounter;
	float _interval = 9999.0f;	// �喂�@�Ɉڍs����܂ł̎��ԁB
};