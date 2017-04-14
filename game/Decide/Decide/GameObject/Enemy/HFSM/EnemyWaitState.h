#pragma once

#include "EnemyState.h"

class EnemyCharacter;

// �G�l�~�[�ҋ@�X�e�[�g�B
class EnemyWaitState : public EnemyState {
public:
	EnemyWaitState(EnemyCharacter* Object);
	~EnemyWaitState();

	void Exit(EnemyCharacter::State next)override;


	// �ҋ@���Ԑݒ�B
	inline void SetInterval(const float time) {
		_Interval = time;
	}
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State now);
private:
	float _TimeCounter;	// ��Ɨp�B
	float _Interval;	// �ҋ@���ԁB
};