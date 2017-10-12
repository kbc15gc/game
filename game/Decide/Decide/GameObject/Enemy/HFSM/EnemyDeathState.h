#pragma once

#include "EnemyState.h"

class Player;

// �G�l�~�[���S�X�e�[�g�B
class EnemyDeathState : public EnemyState {
public:
	EnemyDeathState(EnemyCharacter* Object);
	~EnemyDeathState();
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	inline bool IsPossibleChangeState(EnemyCharacter::State next) {
		return false;
	}
public:
	inline void SetWaitTime(float time) {
		_waitTime = time;
	}
private:
	float _waitTime = 0.0f;	// ���S���[�V�����Đ��I������폜�܂ł̑҂����ԁB
	float _timeCounter = 0.0f;
	bool _isEndAnim = false;	// �Đ��I���t���O�B
};