#pragma once

#include "EnemyState.h"

// �G�l�~�[�ҋ@�X�e�[�g�B
class EnemyWaitState : public EnemyState {
public:
	EnemyWaitState(EnemyCharacter* Object);
	~EnemyWaitState();

	// �ҋ@���Ԑݒ�B
	inline void SetInterval(const float time) {
		_Interval = time;
	}
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override {};

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);
private:
	float _TimeCounter;	// ��Ɨp�B
	float _Interval;	// �ҋ@���ԁB
};